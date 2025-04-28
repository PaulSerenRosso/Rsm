// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaSystem.h"

#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/TweakeableValueCharacterData.h"


// Sets default values for this component's properties
UStaminaSystem::UStaminaSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


void UStaminaSystem::Multicast_InitStaminaSystem_Implementation()
{
	MyPrisoner = Cast<APrisoner>(GetOwner());
}

float UStaminaSystem::GetCurrentStamina()
{
	return CurrentStaminaAmount;
}

float UStaminaSystem::GetCurrentStaminaMax()
{
	return CurrentStaminaMax;
}

// Called when the game starts
void UStaminaSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UStaminaSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!MyPrisoner || !MyPrisoner->HasAuthority())
	{
		return;
	};
	
	if (!IsConsumeStamina && StaminaDelayBeforeRecoveryTimer > 0)
	{
		StaminaDelayBeforeRecoveryTimer -= DeltaTime;
		if (StaminaDelayBeforeRecoveryTimer <= 0)
		{
			CanRecoverStamina = true;
		}
	}
	float NewSprintValue = CurrentStaminaAmount;
	if (CanRecoverStamina && CurrentStaminaAmount < CurrentStaminaMax)
	{
		NewSprintValue += DeltaTime * TweakeableData->StaminaRecoverySpeedInPercent * CurrentStaminaMax;
		SetCurrentStamina(NewSprintValue);
	}
	// ...
	IsConsumeStamina = false;
}

void UStaminaSystem::Server_ConsumeStamina_Implementation(float Value)
{
	if (Value == 0) return;
	float NewValue = CurrentStaminaAmount - Value;
	SetCurrentStamina(NewValue);
	IsConsumeStamina = true;
	CanRecoverStamina = false;
	StaminaDelayBeforeRecoveryTimer = TweakeableData->StaminaDelayBeforeRecovery;
}

void UStaminaSystem::Server_InitStaminaSystem_Implementation()
{
	MyPrisoner = Cast<APrisoner>(GetOwner());
	TweakeableData = MyPrisoner->TweakeableData;
	Server_ResetStamina();
}


void UStaminaSystem::Server_ResetStamina_Implementation()
{
	SetCurrentStaminaMax(TweakeableData->StaminaMax);
	SetCurrentStamina(CurrentStaminaMax);
}


void UStaminaSystem::OnRep_CurrentStaminaAmount() const
{
	OnUpdateStamina.Broadcast();
}

void UStaminaSystem::OnRep_CurrentStaminaMax() const
{
	OnUpdateStamina.Broadcast();
}

void UStaminaSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UStaminaSystem, CurrentStaminaAmount);
	DOREPLIFETIME(UStaminaSystem, CurrentStaminaMax);
}

void UStaminaSystem::SetCurrentStamina(float NewValue)
{
	if (NewValue == CurrentStaminaAmount) return;
	CurrentStaminaAmount = FMath::Clamp(NewValue, 0.0f, CurrentStaminaMax);
	OnRep_CurrentStaminaAmount();
}

void UStaminaSystem::SetCurrentStaminaMax(float NewMaxStaminaValue)
{
	CurrentStaminaMax = NewMaxStaminaValue;
	float NewStaminaValue = FMath::Clamp(CurrentStaminaAmount, 0.0f, CurrentStaminaMax);
	SetCurrentStamina(NewStaminaValue);
	OnRep_CurrentStaminaAmount();
}
