// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorManager.h"

#include "MapInfoManager.h"
#include "ProjectEscapeProto/OnlineGameInstance.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/ElevatorManagerData.h"
#include "ProjectEscapeProto/Objects/LDElements/Elevator.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"


// Sets default values
AElevatorManager::AElevatorManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AElevatorManager::AskGameStart()
{
	OnlineGameMode->AskStartGame();
}

void AElevatorManager::OnGameReadyToStart()
{
	Elevator->Server_StartWaitingTime();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AElevatorManager::AskGameStart,
										   ElevatorManagerData->StartGameElevatorTime, false);
}

void AElevatorManager::OpenElevatorAtLastPhasis()
{
	Elevator->Server_OpenElevator();
}

void AElevatorManager::OnNewPhasisBegan(int CurrentPhasisIndex)
{
	if(CurrentPhasisIndex == OnlineGameMode->GetPhasisCount()-1)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AElevatorManager::OpenElevatorAtLastPhasis,
										   ElevatorManagerData->DelayOpeningAtEndPhase, false);
	}
}


// Called when the game starts or when spawned
void AElevatorManager::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		OnlineGameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
		OnlineGameMode->OnStartGame.AddDynamic(this, &AElevatorManager::OnStartGame);
		OnlineGameMode->PreviousPhasisEnded.AddDynamic(this, &AElevatorManager::OnPreviousPhasisEnded);
		OnlineGameMode->ElevatorManager = this;
		OnlineGameMode->OnGameReadyToStart.AddDynamic(this, &AElevatorManager::OnGameReadyToStart);
		OnlineGameMode->NewPhasisBegan.AddDynamic(this, &AElevatorManager::OnNewPhasisBegan);
		//OnlineGameMode->OnQuestsFinished.AddDynamic(this, &AElevatorManager::SpawnElevator);
	}
}

void AElevatorManager::OnStartGame()
{
	Elevator->Server_OpenElevator();
	Elevator->Server_StopWaitingTime();
}

void AElevatorManager::AskGameEnd()
{
	Elevator->Server_StopWaitingTime();
	OnlineGameMode->GameOver();
}

void AElevatorManager::OnElevatorClosed()
{
	Elevator->Server_StartWaitingTime();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AElevatorManager::AskGameEnd,
										   ElevatorManagerData->EndGameElevatorTime, false);
}

void AElevatorManager::OnPreviousPhasisEnded(int CurrentPhasisIndex)
{
	if(CurrentPhasisIndex == 0)
	{
		Elevator->Server_CloseElevatorImmediately();
	}
	else if (CurrentPhasisIndex == OnlineGameMode->GetPhasisCount()-1)
	{
		Elevator->Server_CloseElevator();
		Elevator->OnElevatorClosed.AddDynamic(this, &AElevatorManager::OnElevatorClosed);
	}

}




