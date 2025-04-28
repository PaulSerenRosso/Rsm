// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowEffectComponent.h"

#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/MoveActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionStateComponent.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/IA/AICharacter.h"

// Sets default values for this component's properties
USlowEffectComponent::USlowEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USlowEffectComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USlowEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->HasAuthority())
	{
		if (SlowTimer > 0.0f)
		{
			SlowTimer -= DeltaTime;
			CurrentSlowRatio = FMath::Lerp(1.0f, MinimumSlowRatio, SlowTimer / MaxSlowTimer);
			if (SlowTimer <= 0.0f)
			{
				RemoveSlow();
			}

			APrisoner* Prisoner = Cast<APrisoner>(GetOwner());
			if (Prisoner)
			{
				UMoveActionComponent* MoveActionComponent = Cast<UMoveActionComponent>(
					Prisoner->GetPlayerActionComponent(UMoveActionComponent::StaticClass()));
				MoveActionComponent->SetSlowRatio(CurrentSlowRatio);
				return;
			}
			AAICharacter* AICharacter = Cast<AAICharacter>(GetOwner());
			if (AICharacter)
			{
				AICharacter->ApplySlow(CurrentSlowRatio);
			}
		}
	}
}

void USlowEffectComponent::SlowCharacter(float Ratio, float Duration)
{
	// UMoveActionComponent* MoveActionComponent = Cast<UMoveActionComponent>(
	// GetOwner()->GetPlayerActionComponent(UMoveActionComponent::StaticClass()));
	// MoveActionComponent->SlowPlayer(Ratio, Duration);

	MinimumSlowRatio = Ratio;
	MaxSlowTimer = Duration;
	SlowTimer = Duration;
}

void USlowEffectComponent::RemoveSlow()
{
	SlowTimer = 0.0f;
	CurrentSlowRatio = 1.0f;

	APrisoner* Prisoner = Cast<APrisoner>(GetOwner());
	if (Prisoner)
	{
		UMoveActionComponent* MoveActionComponent = Cast<UMoveActionComponent>(
			Prisoner->GetPlayerActionComponent(UMoveActionComponent::StaticClass()));
		MoveActionComponent->SetSlowRatio(CurrentSlowRatio);
		return;
	}
	AAICharacter* AICharacter = Cast<AAICharacter>(GetOwner());
	if (AICharacter)
	{
		AICharacter->ApplySlow(CurrentSlowRatio);
	}
}
