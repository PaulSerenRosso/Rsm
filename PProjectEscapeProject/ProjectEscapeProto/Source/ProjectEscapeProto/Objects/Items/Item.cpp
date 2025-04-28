// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/ActorComponents/InventorySystem.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//InteractionRoot->SetupAttachment(ItemMesh);
}

FVector AItem::GetCustomScaleOnHand() const
{
	return CustomScaleOnHand;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	SetReplicateMovement(true);
	Super::BeginPlay();
}

void AItem::OnRep_IsInvisible()
{
	SetActorHiddenInGame(IsInvisible);
}

bool AItem::CanBeReused()
{
	return CurrentTimeToBeReusable <= 0;
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentTimeToBeReusable > 0)
	{
		CurrentTimeToBeReusable -= DeltaTime;
		if (CurrentTimeToBeReusable <= 0) CurrentTimeToBeReusable = 0;
	}
}

void AItem::UseItem()
{
}

void AItem::StopUsingItem()
{
}

void AItem::ServerSetVisibility_Implementation(bool NeedToBeInvisible)
{
	IsInvisible = NeedToBeInvisible;

	OnRep_IsInvisible();
}

bool AItem::ServerSetVisibility_Validate(bool NeedToBeInvisible)
{
	return true;
}

void AItem::SetVisibilityOfObj(bool NeedToBeInvisible)
{
	if (HasAuthority())
	{
		IsInvisible = NeedToBeInvisible;
		OnRep_IsInvisible();
	}
	else
	{
		ServerSetVisibility(NeedToBeInvisible);
	}
}

bool AItem::CanInteract_Implementation(AActor* Actor)
{
	APrisoner* Prisoner = Cast<APrisoner>(Actor);
	return Prisoner != nullptr && IncompleteInteraction && CanBeUsed;
}

bool AItem::TryInteract_Implementation(AActor* Actor)
{
	APrisoner* Prisoner = Cast<APrisoner>(Actor);
	if (IncompleteInteraction && Prisoner != nullptr)
	{
		Prisoner->GrabItem(this);
		return true;
	}
	return false;
}

void AItem::ConsumeItem()
{
	if (ObjectOwner)
	{
		UInventorySystem* Inventory = ObjectOwner->GetComponentByClass<UInventorySystem>();
		if (Inventory)
		{
			Inventory->Server_SetFirstItemSlot(nullptr);
			Inventory->Client_AddItem(nullptr);
		}
			
			
	
		SetActorScale3D(FVector(1, 1, 1));
	}
}

void AItem::OnBeginToInteract_Implementation()
{
	void OnBeginToInteractBP();
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItem, IsInvisible);
}
