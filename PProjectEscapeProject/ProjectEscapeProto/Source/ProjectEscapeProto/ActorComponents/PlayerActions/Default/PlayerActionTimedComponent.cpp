// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionTimedComponent.h"

#include "ProjectEscapeProto/Characters/Prisoner.h"

// Sets default values for this component's properties
UPlayerActionTimedComponent::UPlayerActionTimedComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPlayerActionTimedComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle,this, &UPlayerActionTimedComponent::EndTimerAction, GetCurrentTime());
	MyPrisoner->CanStartPlayersActions = false;
	Super::MyServer_ExecuteAction(Payload, Actors);
}

void UPlayerActionTimedComponent::MyServer_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	MyPrisoner->CanStartPlayersActions = true;
	ActionTimerHandle.Invalidate();
	Super::MyServer_StopAction(Payload, Actors);
}

void UPlayerActionTimedComponent::EndTimerAction()
{
	AskStopAction();
}

float UPlayerActionTimedComponent::GetCurrentTime()
{
	return 0;
}


// Called when the game starts
void UPlayerActionTimedComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


