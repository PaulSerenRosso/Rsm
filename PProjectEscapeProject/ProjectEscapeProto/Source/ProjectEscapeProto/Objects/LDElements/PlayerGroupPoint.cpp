// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGroupPoint.h"
#include "Components/BoxComponent.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"


// Sets default values
APlayerGroupPoint::APlayerGroupPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	bReplicates = true;
	GroupColliderOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("Group Collider Overlap"));
	GroupColliderOverlap->SetupAttachment(RootComponent);
	GroupColliderOverlap->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GroupColliderOverlap->SetCollisionResponseToAllChannels(ECR_Ignore);
	GroupColliderOverlap->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetSimulatePhysics(false);
	GroupColliderOverlap->OnComponentBeginOverlap.AddDynamic(this, &APlayerGroupPoint::OnBeginOverlap);
	GroupColliderOverlap->OnComponentEndOverlap.AddDynamic(this, &APlayerGroupPoint::OnEndOverlap);

}

// Called when the game starts or when spawned
void APlayerGroupPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerGroupPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerGroupPoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if(!HasAuthority() && !IncompleteInteraction) return;
	APrisoner* Prisoner = Cast<APrisoner>(OtherActor);
	if(Prisoner)
	{
		PrisonersGrouped.Add(Prisoner);
		OnPlayerJoin.Broadcast(Prisoner);
		
	}
}

void APlayerGroupPoint::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!IncompleteInteraction) return;
	APrisoner* Prisoner = Cast<APrisoner>(OtherActor);
	if(Prisoner)
	{
		PrisonersGrouped.Remove(Prisoner);
		OnPlayerLeave.Broadcast(Prisoner);
	}
}

