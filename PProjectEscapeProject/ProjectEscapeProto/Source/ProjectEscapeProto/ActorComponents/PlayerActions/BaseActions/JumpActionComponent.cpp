// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpActionComponent.h"

#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/TweakeableValueCharacterData.h"


// Sets default values for this component's properties
UJumpActionComponent::UJumpActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
		
	// ...
}


// Called when the game starts
void UJumpActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UJumpActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if(!CanExecuteAction()) return;
	Super::MyServer_ExecuteAction(Payload, Actors);
	MyPrisoner->GenerateSound(MyPrisoner->TweakeableData->JumpNoiseEmitted);
	Multicast_ExecuteAction(Payload, Actors);
}

void UJumpActionComponent::MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyMulticast_ExecuteAction(Payload, Actors);
	if(MyPrisoner->IsLocallyControlled())
	{
		MyPrisoner->Jump();
		
	}
}


// Called every frame
void UJumpActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

