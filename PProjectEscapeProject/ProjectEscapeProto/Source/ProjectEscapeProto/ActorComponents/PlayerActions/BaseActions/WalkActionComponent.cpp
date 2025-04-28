// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkActionComponent.h"


// Sets default values for this component's properties
UWalkActionComponent::UWalkActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWalkActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if(!CanExecuteAction()) return;
	Super::MyServer_ExecuteAction(Payload, Actors);
	Multicast_ExecuteAction(Payload, Actors);
}

void UWalkActionComponent::MyServer_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyServer_StopAction(Payload, Actors);
	Multicast_StopAction(Payload, Actors);
}

void UWalkActionComponent::MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyMulticast_ExecuteAction(Payload, Actors);
}

void UWalkActionComponent::MyMulticast_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyMulticast_StopAction(Payload, Actors);
}

EPlayerActionState UWalkActionComponent::GetPlayerActionState()
{
	return EPlayerActionState::IsWalkingQuiet;
}


// Called when the game starts
void UWalkActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWalkActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

