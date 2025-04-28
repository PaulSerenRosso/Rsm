// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentalTrap.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/ActorComponents/TemporaryEffects/TemporaryEffect.h"
#include "ProjectEscapeProto/DataAssets/TweakValueTrapData.h"

// Sets default values
AEnvironmentalTrap::AEnvironmentalTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnvironmentalTrap::BeginPlay()
{
	Super::BeginPlay();
}

void AEnvironmentalTrap::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnvironmentalTrap, IsCurrentlyActivated);
}

bool AEnvironmentalTrap::CanInteract_Implementation(AActor* Actor)
{
	return !IsCurrentlyActivated;
}

bool AEnvironmentalTrap::TryInteract_Implementation(AActor* Actor)
{
	//please perforce delete this line below

	
	IsCurrentlyActivated = true;

	if (TrapData->CanBeActivateMultipleTimes)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			IsCurrentlyActivated = false;
		}, TrapData->DelayBeforeReusingTrap, false);
	}

	FTimerHandle TimerHandleUse;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleUse, [this, Actor]()
	{
		ActivateEnvironmentalTrapBP();
	}, TrapData->TimerBeforeActivateTrap, false);

	return IIInteractable::TryInteract_Implementation(Actor);
}

void AEnvironmentalTrap::StunCharacters(TArray<AActor*> Actors)
{
	for (auto OtherActor : Actors)
	{
		if (!OtherActor) return;
		ACharacter* Ch = Cast<ACharacter>(OtherActor);
		if (!Ch) return;

		UTemporaryEffect* TemporaryEffectComponent = Cast<UTemporaryEffect>(
			Ch->GetComponentByClass(UTemporaryEffect::StaticClass()));
		if (!TemporaryEffectComponent) return;

		TemporaryEffectComponent->StunCharacter(TrapData->StunDuration);
		if (TrapData->NeedToBeDestroy)
		{
			Destroy();
		}
	}
}

void AEnvironmentalTrap::SlowCharacters(TArray<AActor*> Actors)
{
	for (auto OtherActor : Actors)
	{
		if (!OtherActor) return;
		ACharacter* Ch = Cast<ACharacter>(OtherActor);
		if (!Ch) return;

		UTemporaryEffect* TemporaryEffectComponent = Cast<UTemporaryEffect>(
			Ch->GetComponentByClass(UTemporaryEffect::StaticClass()));
		if (!TemporaryEffectComponent) return;

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, TemporaryEffectComponent]()
		                                       {
			                                       TemporaryEffectComponent->SlowCharacter(
				                                       TrapData->MinimumRatioOfSpeed,
				                                       TrapData->DurationSlowTrap);
			                                       if (TrapData->NeedToBeDestroy)
				                                       Destroy();
		                                       },
		                                       TrapData->StunDuration, false);
	}
}

void AEnvironmentalTrap::BlindCharacters(TArray<AActor*> Actors)
{
	for (auto OtherActor : Actors)
	{
		if (!OtherActor) return;
		ACharacter* Ch = Cast<ACharacter>(OtherActor);
		if (!Ch) return;

		UTemporaryEffect* TemporaryEffectComponent = Cast<UTemporaryEffect>(
			Ch->GetComponentByClass(UTemporaryEffect::StaticClass()));
		if (!TemporaryEffectComponent) return;

		//TemporaryEffectComponent->BlindCharacter();
		
		if (TrapData->NeedToBeDestroy)
		{
			Destroy();
		}
	}
}


// Called every frame
void AEnvironmentalTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
