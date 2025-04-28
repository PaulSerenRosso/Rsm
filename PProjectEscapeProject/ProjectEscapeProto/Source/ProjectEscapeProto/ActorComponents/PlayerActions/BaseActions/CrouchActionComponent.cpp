// Fill out your copyright notice in the Description page of Project Settings.


#include "CrouchActionComponent.h"

#include "GameFramework/PawnMovementComponent.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"


// Sets default values for this component's properties
UCrouchActionComponent::UCrouchActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCrouchActionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCrouchActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if (!CanExecuteAction()) return;
	
	Super::MyServer_ExecuteAction(Payload, Actors);
	Multicast_ExecuteAction(Payload, Actors);
}

void UCrouchActionComponent::MyServer_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyServer_StopAction(Payload, Actors);
	Multicast_StopAction(Payload, Actors);
}

void UCrouchActionComponent::MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if(MyPrisoner->IsLocallyControlled())
	{
		MyPrisoner->Crouch();
	}
	Super::MyMulticast_ExecuteAction(Payload, Actors);
}

// void UCrouchActionComponent::MyMulticast_StopAction(const TArray<uint8>& Payload,
// 	const TArray<AActor*>& Actors)
// {
// 	if(MyPrisoner->IsLocallyControlled())
// 	{
// 		MyPrisoner->UnCrouch();
// 		
// 	}
// 	Super::MyMulticast_StopAction(Payload, Actors);
// }

void UCrouchActionComponent::MyMulticast_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyMulticast_StopAction(Payload, Actors);
	if (MyPrisoner->IsLocallyControlled())
	{
		MyPrisoner->UnCrouch();
	}
}

bool UCrouchActionComponent::CanExecuteAction()
{
	return Super::CanExecuteAction() && MyPrisoner->GetMovementComponent()->IsMovingOnGround();
}

EPlayerActionState UCrouchActionComponent::GetPlayerActionState()
{
	return EPlayerActionState::IsCrouching;
}


// Called every frame
void UCrouchActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

