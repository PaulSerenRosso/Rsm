// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDoorPathfinder.h"
#include "Components/BoxComponent.h"
#include "ProjectEscapeProto/IA/AICharacter.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"

AAIDoorPathfinder::AAIDoorPathfinder()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
}

void AAIDoorPathfinder::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAIDoorPathfinder::SwitchRoom);
}

void AAIDoorPathfinder::SwitchRoom(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAICharacter* AICharacter = Cast<AAICharacter>(OtherActor);
	if (!AICharacter) return;
	
	AHearingNeighbourController *Controller = Cast<AHearingNeighbourController>(AICharacter->GetController());
	if (!Controller) return;
	
	Controller->SetCurrentRoom(Room);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Room: %d"), static_cast<uint8>(Room)));
}

void AAIDoorPathfinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
