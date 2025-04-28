// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"

#include "PlayerGroupPoint.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"


// Sets default values
AElevator::AElevator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	PlayerGroupActorComponent = CreateDefaultSubobject<UChildActorComponent>("PlayerGroupPointSpawner");
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	PlayerGroupActorComponent->SetupAttachment(RootComponent);
	PlayerGroupActorComponent->SetChildActorClass(APlayerGroupPoint::StaticClass());

}

void AElevator::OnPrisonerLeavedElevator(APrisoner* Player)
{
	OnPrisonersInElevatorUpdated.Broadcast(PlayerGroupPoint->PrisonersGrouped);
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	PlayerGroupPoint = Cast<APlayerGroupPoint>(PlayerGroupActorComponent->GetChildActor());
	PlayerGroupPoint->OnPlayerJoin.AddDynamic(this, &AElevator::OnPrisonerEnteredElevator);
	PlayerGroupPoint->OnPlayerLeave.AddDynamic(this, &AElevator::OnPrisonerLeavedElevator);
	
}

void AElevator::OnPrisonerEnteredElevator(APrisoner* Prisoner)
{
	OnPrisonersInElevatorUpdated.Broadcast(PlayerGroupPoint->PrisonersGrouped);
}


// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AElevator::Server_StartWaitingTime_Implementation()
{
	BP_Server_StartWaitingTime();
	Multicast_StartWaitingTime();
}

void AElevator::Multicast_StartWaitingTime_Implementation()
{
	BP_Multicast_StartWaitingTime();
}

void AElevator::Server_StopWaitingTime_Implementation()
{
	BP_Server_StopWaitingTime();
	Multicast_StopWaitingTime();
}

void AElevator::Multicast_StopWaitingTime_Implementation()
{
	BP_Multicast_StopWaitingTime();
}




void AElevator::Multicast_CloseElevatorImmediately_Implementation()
{
	BP_Multicast_CloseImmediately();
}

void AElevator::Multicast_OpenElevator_Implementation()
{
	BP_Multicast_OpenElevator();
}

void AElevator::Multicast_CloseElevator_Implementation()
{
	BP_Multicast_CloseElevator();
}

void AElevator::Server_CloseElevatorImmediately_Implementation()
{
	BP_Server_CloseImmediately();
	Multicast_CloseElevatorImmediately();
}

void AElevator::Server_OpenElevator_Implementation()
{
	BP_Server_OpenElevator();
	Multicast_OpenElevator();
}

void AElevator::Server_CloseElevator_Implementation()
{
	BP_Server_CloseElevator();
	Multicast_CloseElevator();
}




