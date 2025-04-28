// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/ActorComponents/InventorySystem.h"
#include "ProjectEscapeProto/DataAssets/TweakValueTrapData.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"

// Sets default values
ATrap::ATrap()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetActorEnableCollision(true);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);


	BoxComponent->SetupAttachment(ItemMesh);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnBoxBeginOverlap);
}

void ATrap::UseItem()
{
	
	APrisoner* ObjectOwnerPlayer = Cast<APrisoner>(ObjectOwner);
	if (!ObjectOwnerPlayer) return;
	FVector NewLocation = ObjectOwnerPlayer->GetCameraPosition() + ObjectOwnerPlayer->GetCameraForwardVector() * 200;
	
	char Succeed = UFunctionsUtilities::GetPointOnFloor(ObjectOwnerPlayer, MaxDistanceActivateTrap, NewLocation);

	if (Succeed != 0) return;

	Super::UseItem();
	ConsumeItem();
	IncompleteInteraction = true;
	Server_ArmTrap();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorRotation(FRotator(0, 0, 0));

	SetActorLocation(NewLocation);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		(HasAuthority()) ? Multicast_ArmTrap() : Server_ArmTrap();
		SetActorHiddenInGame(false);
		UBoxComponent* BoxC = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
		if (BoxC) BoxC->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UMeshComponent* MeshC = Cast<UMeshComponent>(GetComponentByClass(UMeshComponent::StaticClass()));
		if (MeshC) MeshC->SetSimulatePhysics(true);
	}, 0.2f, false);
	BP_UseItem();
}

bool ATrap::CanInteract_Implementation(AActor* Actor)
{
	if (IsArmed) return false;
	return Super::CanInteract_Implementation(Actor);
}

void ATrap::BeginPlay()
{
	Super::BeginPlay();
	MaxDistanceActivateTrap = TweakeableData->MaxDistanceActivateTrap;
}

void ATrap::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (IsArmed)
	{
		(HasAuthority()) ? Multicast_DisarmTrap() : Server_DisarmTrap();
		ActivateTrap(OtherActor);
	}
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrap::ArmTrap()
{
	BP_ArmTrap();
}

bool ATrap::Server_ArmTrap_Validate()
{
	return true;
}

void ATrap::Server_ArmTrap_Implementation()
{
	OnTrapArmed.Broadcast();
	Multicast_ArmTrap();
}

void ATrap::Multicast_ArmTrap_Implementation()
{
	IsArmed = true;
	ArmTrap();
}

void ATrap::Server_DisarmTrap_Implementation()
{
	Multicast_DisarmTrap();
}

bool ATrap::Server_DisarmTrap_Validate()
{
	return true;
}

void ATrap::Multicast_DisarmTrap_Implementation()
{
	IsArmed = false;
}

void ATrap::ActivateTrap(AActor* OtherActor)
{
	// Do something
	BP_ActivateTrap(OtherActor);
}

void ATrap::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATrap, IsArmed);
}
