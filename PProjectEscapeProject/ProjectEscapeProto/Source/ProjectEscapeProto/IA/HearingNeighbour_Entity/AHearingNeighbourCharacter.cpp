// Fill out your copyright notice in the Description page of Project Settings.


#include "AHearingNeighbourCharacter.h"

#include "HearingNeighbourController.h"
#include "JeremiePhase.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"

AAHearingNeighbourCharacter::AAHearingNeighbourCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	WalkAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WalkAudioComponent"));
}

void AAHearingNeighbourCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (auto Phase : Phases)
	{
		Phase->GetDefaultObject<UJeremiePhase>()->
		       SetOwnerController(Cast<AHearingNeighbourController>(GetController()));
	}
}

void AAHearingNeighbourCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	if (WalkAudioComponent && GetCharacterMovement()->IsMovingOnGround() && GetCapsuleComponent()->GetComponentVelocity().Length() > 0.1f)
	{
		if (!WalkAudioComponent) return;

		if (!WalkAudioComponent->IsPlaying())
			WalkAudioComponent->Play();
	}
	else if (WalkAudioComponent && WalkAudioComponent->IsPlaying())
	{
		WalkAudioComponent->Stop();
	}

	if (Phases.Num() > 0)
	{
		if (Phases[0]->GetDefaultObject<UJeremiePhase>()->TimerBeforeTrigger <= ElapsedTime)
		{
			Phases[0]->GetDefaultObject<UJeremiePhase>()->OnPhaseStart();
			Phases.RemoveAt(0);
		}
	}
}

void AAHearingNeighbourCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAHearingNeighbourCharacter::OnAlert(APrisoner* Target)
{
	OnDetectPlayer.Broadcast(Target);
	BP_OnAlert(Target);
}
