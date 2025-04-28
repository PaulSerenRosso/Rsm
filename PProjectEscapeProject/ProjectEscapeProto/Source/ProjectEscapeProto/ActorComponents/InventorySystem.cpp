// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/Interfaces/Grabable.h"
#include "ProjectEscapeProto/Objects/Items/Item.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"

// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventorySystem::Server_SetSecondItemSlot_Implementation(AItem* Item)
{
	SecondItemSlot = Item;
	OnRep_SecondItemSlot();
}

void UInventorySystem::DropInventory()
{
	if (FirstItemSlot)
	{
		TryRemoveItem(FirstItemSlot, Cast<APrisoner>(GetOwner()), true, 100);
	}
	if (SecondItemSlot)
	{
		TryRemoveItem(SecondItemSlot, Cast<APrisoner>(GetOwner()), false, 100);
	}
}

void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();
	MyOwner = Cast<APawn>(GetOwner());
}

// Called every frame
void UInventorySystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MyOwner.IsValid() && MyOwner->IsLocallyControlled() && PrevisuItem.IsValid())
	{
		FVector NewLocation;
		char Succeed = UFunctionsUtilities::GetPointOnFloor(Cast<APrisoner>(GetOwner()), 400, NewLocation);
		if (!PrevisuItem.IsValid()) return;
		if (Succeed != 2)
		{
			PrevisuItem->SetActorLocation(NewLocation);
			PrevisuItem->SetActorRotation(FRotator::ZeroRotator);
			if (Succeed == 0)
			{
				Cast<APrisoner>(GetOwner())->SwapHintPrevisualisationBP(true);
				PrevisuItem->ValidPrevisuMaterialBP();
			}
			else
			{
				PrevisuItem->InvalidPrevisuMaterialBP();
				Cast<APrisoner>(GetOwner())->SwapHintPrevisualisationBP(false);
			}
			PrevisuItem->SetActorHiddenInGame(false);
		}
		else
		{
			PrevisuItem->SetActorHiddenInGame(true);
			Cast<APrisoner>(GetOwner())->SwapHintPrevisualisationBP(false);
		}
	}
}

bool UInventorySystem::TryAddItem(AItem* Item, APrisoner* Prisoner, bool AddToFirstSlot)
{
	if (!Item->InventoryItem) return false;

	if (AddToFirstSlot)
	{
		if (FirstItemSlot == nullptr)
		{
			Server_SetFirstItemSlot(Item);
			AttachItem(Item, Prisoner, false);
			Item->SetInteractable(false);
			Client_AddItem(FirstItemSlot);
			RefreshHandItem(Prisoner);
			return true;
		}
		return false;
	}
	if (SecondItemSlot == nullptr)
	{
		Server_SetSecondItemSlot(Item);
		Item->SetInteractable(false);
		AttachItem(Item, Prisoner, true);
		RefreshHandItem(Prisoner);
		return true;
	}

	TryRemoveItem(FirstItemSlot, Prisoner, true);
	TryAddItem(Item, Prisoner, true);
	return false;
}

void UInventorySystem::Client_AddItem_Implementation(AItem* Item)
{
	if (PrevisuItem.IsValid())
	{
		Cast<APrisoner>(GetOwner())->SwapHintPrevisualisationBP(false);
		PrevisuItem->Destroy();
	}
	if (!Item || !Item->HavePrevisu) return;
	PrevisuItem = GetWorld()->SpawnActor<AItem>(Item->GetClass());
	PrevisuItem->SetReplicates(false);
	PrevisuItem->SetReplicateMovement(false);
	PrevisuItem->SetActorEnableCollision(false);
}


bool UInventorySystem::TryRemoveItem(AItem* Item, APrisoner* Prisoner, bool RemoveToFirstSlot, const float DropDistance)
{
	if (RemoveToFirstSlot)
	{
		if (FirstItemSlot != nullptr)
		{
			Server_SetFirstItemSlot(nullptr);
			Item->SetInteractable(true);
			DetachItem(Item, Prisoner, DropDistance);
			Client_AddItem(nullptr);

			return true;
		}
		return false;
	}
	if (SecondItemSlot != nullptr)
	{
		Server_SetSecondItemSlot(nullptr);
		Item->SetInteractable(true);
		DetachItem(Item, Prisoner, DropDistance);
		return true;
	}
	return false;
}

void UInventorySystem::Server_SetFirstItemSlot_Implementation(AItem* Item)
{
	FirstItemSlot = Item;
	OnRep_FirstItemSlot();
}

void UInventorySystem::AttachItem_Implementation(AItem* Actor, APrisoner* Prisoner, bool IsInvisible)
{
	if (!Actor || !Prisoner) return;

	UBoxComponent* BoxC = Cast<UBoxComponent>(Actor->GetComponentByClass(UBoxComponent::StaticClass()));
	if (BoxC) BoxC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMeshComponent* MeshC = Cast<UMeshComponent>(Actor->GetComponentByClass(UMeshComponent::StaticClass()));
	if (MeshC)
	{
		MeshC->SetSimulatePhysics(false);
		MeshC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	USphereComponent* SphereC = Cast<USphereComponent>(Actor->GetComponentByClass(USphereComponent::StaticClass()));
	if (SphereC) SphereC->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Prisoner->IsLocallyControlled())
	{
		AttachOnHand(Actor, Prisoner, Prisoner->GetMesh(), IsInvisible);
	}
	else
	{
		AttachOnHand(Actor, Prisoner, Prisoner->MeshThirdPerson, IsInvisible);
	}
}

void UInventorySystem::RefreshHandItem(APrisoner* Prisoner)
{
	FTimerHandle FirstSwapHandle;

	GetWorld()->GetTimerManager().SetTimer(FirstSwapHandle, [this, Prisoner]()
	{
		Prisoner->SwapSlotInventory();
		FTimerHandle SecondSwapHandle;
		GetWorld()->GetTimerManager().SetTimer(SecondSwapHandle, [this, Prisoner]()
		{
			Prisoner->SwapSlotInventory();
		}, 0.1f, false);
	}, 0.1f, false);
}

void UInventorySystem::AttachOnHand(AItem* Actor, APrisoner* Prisoner, USkeletalMeshComponent* ParentComponent,
                                    bool IsInvisible)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Actor, Prisoner, IsInvisible, ParentComponent]()
	{
		bool Success = Actor->AttachToComponent(ParentComponent,
		                                        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		                                        Actor->GetSocketHandName());
		Actor->ServerSetVisibility(IsInvisible);
		Actor->SetActorScale3D(Actor->GetCustomScaleOnHand());
		Actor->ObjectOwner = Prisoner;

		if (!Success) AttachOnHand(Actor, Prisoner, ParentComponent, IsInvisible);
	}, 0.03f, false);
}

void UInventorySystem::OnRep_FirstItemSlot()
{
	OnInventoryChanged.Broadcast(FirstItemSlot, SecondItemSlot);
}

void UInventorySystem::OnRep_SecondItemSlot()
{
	OnInventoryChanged.Broadcast(FirstItemSlot, SecondItemSlot);
}


void UInventorySystem::DetachItem_Implementation(AItem* Item, APrisoner* Prisoner, float DropDistance)
{
	if (!Item || !Prisoner) return;
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->SetActorRotation(FRotator(0, 0, 0));
	Item->SetActorLocation(Prisoner->GetActorLocation() + Prisoner->GetActorForwardVector() * DropDistance);
	Item->SetActorScale3D(Item->CustomScaleOnDrop);
	Item->SetActorHiddenInGame(false);
	if (DropDistance != 0)
	{
		FCollisionQueryParams RV_TraceParams(FName(TEXT("RV_Trace")), true, Prisoner);
		FHitResult RV_Hit;
		FVector Start = Prisoner->GetActorLocation();
		FVector End = Start + (Prisoner->GetActorForwardVector() * DropDistance);
		bool bHit = GetWorld()->LineTraceSingleByChannel(RV_Hit, Start, End, ECC_Camera, RV_TraceParams);
		if (bHit)
		{
			if (bHit && RV_Hit.GetActor())
			{
				Item->SetActorLocation(RV_Hit.ImpactPoint);
			}
		}
		else
		{
			Item->SetActorLocation(Prisoner->GetActorLocation() + Prisoner->GetActorForwardVector() * DropDistance);
		}
	}
	else
	{
		Item->SetActorLocation(Prisoner->GetActorLocation() + Prisoner->GetActorForwardVector() * DropDistance);
	}


	UBoxComponent* BoxC = Cast<UBoxComponent>(Item->GetComponentByClass(UBoxComponent::StaticClass()));
	if (BoxC) BoxC->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	UMeshComponent* MeshC = Cast<UMeshComponent>(Item->GetComponentByClass(UMeshComponent::StaticClass()));
	if (MeshC)
	{
		MeshC->SetSimulatePhysics(true);
		MeshC->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
	USphereComponent* SphereC = Cast<USphereComponent>(Item->GetComponentByClass(USphereComponent::StaticClass()));
	if (SphereC) SphereC->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Item->ObjectOwner = nullptr;
}


void UInventorySystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventorySystem, FirstItemSlot);
	DOREPLIFETIME(UInventorySystem, SecondItemSlot);
}
