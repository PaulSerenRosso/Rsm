// Fill out your copyright notice in the Description page of Project Settings.


#include "StunEffectComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectEscapeProto/ActorComponents/InventorySystem.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionStateComponent.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/IA/AICharacter.h"

UStunEffectComponent::UStunEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UStunEffectComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UStunEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->HasAuthority())
	{
		for (int i = StunTimer.Num() - 1; i >= 0; i--)
		{
			StunTimer[i] -= DeltaTime;
			if (StunTimer[i] <= 0.0f)
			{
				StunTimer.RemoveAt(i);
				if (StunTimer.Num() == 0)
				{
					RemoveStun();
				}
				else if (AIMovement.IsValid())
				{
					AIMovement->MaxWalkSpeed = 0;
				}
			}
		}
	}
}

void UStunEffectComponent::StunCharacter(float StunTime)
{
	float NewStunTime = StunTime;
	StunTimer.Add(NewStunTime);
	if (StunTimer.Num() > 1) return;
	APrisoner* Prisoner = Cast<APrisoner>(GetOwner());
	if (Prisoner)
	{
		Prisoner->Inventory->TryRemoveItem(Prisoner->Inventory->FirstItemSlot, Prisoner, true, 0);
		Prisoner->CanStartPlayersActions = false;
		Prisoner->GetCharacterMovement()->Velocity = FVector(0, 0, 0);
		for (auto PlayerAction : Prisoner->CurrentPlayerActions)
		{
			UPlayerActionStateComponent* PlayerActionStateComponent = Cast<UPlayerActionStateComponent>(PlayerAction.Value);
			if (PlayerActionStateComponent != nullptr)
			{
				if (PlayerActionStateComponent->IsActiveState())
				{
					PlayerActionStateComponent->AskStopAction();
				}
			}
		}
	}
	AAICharacter* AICharacter = Cast<AAICharacter>(GetOwner());
	if (AICharacter)
	{
		AIMovement = AICharacter->GetCharacterMovement();
		if (!AIMovement.IsValid()) return;
		AIMovement->MaxWalkSpeed = 0;
	}

}

void UStunEffectComponent::RemoveStun()
{
	APrisoner* Prisoner = Cast<APrisoner>(GetOwner());
	if (Prisoner)
	{
		Prisoner->CanStartPlayersActions = true;
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	AAICharacter* AICharacter = Cast<AAICharacter>(GetOwner());
	if (AICharacter)
	{
		if (!AIMovement.IsValid()) return;
		AIMovement->MaxWalkSpeed = AICharacter->BasicMaxWalkSpeed;
	}
}


