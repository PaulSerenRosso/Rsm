// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnActor.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/InteractActionComponent.h"

// Sets default values
ASpawnActor::ASpawnActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetIsReplicated(true);

	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	ItemMesh->SetCollisionResponseToChannel(ECC_Interactable, ECR_Block);
}

void ASpawnActor::DeactivateInteraction()
{
	if (!StillInteractable)
		IncompleteInteraction = false;
	CanBeUsed = false;
	StillInteractable = false;
	BP_DeactivateInteraction();
}

// Called when the game starts or when spawned
void ASpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawnActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASpawnActor, IncompleteInteraction);
	DOREPLIFETIME(ASpawnActor, CanBeUsed);
}

// Called every frame
void ASpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnActor::ActivateWallhack_Implementation()
{
	if (ItemMesh)
	{
		ItemMesh->SetRenderCustomDepth(true);
	}
}


