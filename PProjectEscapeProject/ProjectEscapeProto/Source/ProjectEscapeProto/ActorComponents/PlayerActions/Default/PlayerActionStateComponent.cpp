// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionStateComponent.h"

#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"


// Sets default values for this component's properties
UPlayerActionStateComponent::UPlayerActionStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPlayerActionStateComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyServer_ExecuteAction(Payload, Actors);
}

void UPlayerActionStateComponent::MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyMulticast_ExecuteAction(Payload, Actors);
	UFunctionsUtilities::BinaryMask_SetValue(MyPrisoner->PlayerActionStateMask, StaticCast<int32>(GetPlayerActionState()), true);
	ActiveState = true;
}

void UPlayerActionStateComponent::MyServer_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
}

void UPlayerActionStateComponent::AskStopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Server_StopAction(Payload, Actors);
}

void UPlayerActionStateComponent::Server_StopAction_Implementation(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	MyServer_StopAction(Payload, Actors);
	EventServerStopAction(Payload,  Actors);
}


void UPlayerActionStateComponent::Multicast_StopAction_Implementation(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if (!MyPrisoner || !IsInit) return;
	MyMulticast_StopAction(Payload, Actors);
	UFunctionsUtilities::BinaryMask_SetValue(MyPrisoner->PlayerActionStateMask, StaticCast<int32>(GetPlayerActionState()), false);
	EventMulticastStopAction(Payload, Actors);
	MyMulticast_StopAction(Payload, Actors);
	OnPlayerActionStopped.Broadcast(Payload, Actors);
	ActiveState = false;
}



// Called when the game starts
void UPlayerActionStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

EPlayerActionState UPlayerActionStateComponent::GetPlayerActionState()
{
	return EPlayerActionState::None;
}

void UPlayerActionStateComponent::MyMulticast_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
}


// Called every frame
void UPlayerActionStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

