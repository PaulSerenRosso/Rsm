// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintActionComponent.h"

#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/ActorComponents/StaminaSystem.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/TweakeableValueCharacterData.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"


// Sets default values for this component's properties
USprintActionComponent::USprintActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USprintActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void USprintActionComponent::InitMulticastAction(APrisoner* InPrisoner)
{
	Super::InitMulticastAction(InPrisoner);
	TweakeableData = MyPrisoner->TweakeableData;
}


// Called every frame
void USprintActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!MyPrisoner) return;

	if (!MyPrisoner->HasAuthority()) return;
	bool IsSprinting = UFunctionsUtilities::BinaryMask_HaveValue(MyPrisoner->PlayerActionStateMask,
												  StaticCast<int32>(EPlayerActionState::IsSprinting));

	if (IsSprinting && MyPrisoner->GetVelocity().Length() > 0.1f)
	{
		
		StaminaSystem->Server_ConsumeStamina(DeltaTime * TweakeableData->SprintBurnCostSpeed);
		if (StaminaSystem->GetCurrentStamina() == 0)
		{
			AskStopAction();
		}
	}
	// ...
}

EPlayerActionState USprintActionComponent::GetPlayerActionState()
{
	return EPlayerActionState::IsSprinting;
}

void USprintActionComponent::InitServerAction(APrisoner* InPrisoner)
{
	Super::InitServerAction(InPrisoner);
	StaminaSystem = MyPrisoner->Stamina;

}

void USprintActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if (!CanExecuteAction()) return;
	StaminaSystem->Server_ConsumeStamina(TweakeableData->SprintInitialBurnCost);
	Super::MyServer_ExecuteAction(Payload, Actors);
	Multicast_ExecuteAction(Payload, Actors);
}

void USprintActionComponent::MyServer_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if (!UFunctionsUtilities::BinaryMask_HaveValue(
		MyPrisoner->PlayerActionStateMask, StaticCast<int32>(EPlayerActionState::IsSprinting)))
		return;
	Super::MyServer_StopAction(Payload, Actors);
	Multicast_StopAction(Payload, Actors);
}

bool USprintActionComponent::CanExecuteAction()
{
	return Super::CanExecuteAction() && StaminaSystem->GetCurrentStamina() >= TweakeableData->SprintInitialBurnCost;
}



