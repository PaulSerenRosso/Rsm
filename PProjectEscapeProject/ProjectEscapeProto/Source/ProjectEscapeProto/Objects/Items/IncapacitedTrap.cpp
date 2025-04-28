// Fill out your copyright notice in the Description page of Project Settings.


#include "IncapacitedTrap.h"

#include "GameFramework/Character.h"
#include "ProjectEscapeProto/ActorComponents/TemporaryEffects/TemporaryEffect.h"
#include "ProjectEscapeProto/DataAssets/TweakValueTrapData.h"

void AIncapacitedTrap::BeginPlay()
{
	Super::BeginPlay();
	
	HavePrevisu = true;
}

void AIncapacitedTrap::ArmTrap()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (ItemMesh)
		{
			ItemMesh->MarkRenderStateDirty();
		}
	}, 0.01f, false);
	Super::ArmTrap();
}

void AIncapacitedTrap::Multicast_DisarmTrap()
{
	Super::Multicast_DisarmTrap();
}

void AIncapacitedTrap::ActivateTrap(AActor* OtherActor)
{
	
	//SetActorHiddenInGame(true);
	if (!OtherActor) return;
	ACharacter* Ch = Cast<ACharacter>(OtherActor);
	if (!Ch) return;

	UTemporaryEffect* TemporaryEffectComponent = Cast<UTemporaryEffect>(
		Ch->GetComponentByClass(UTemporaryEffect::StaticClass()));
	if (!TemporaryEffectComponent) return;

	OnActivateTrap.Broadcast();
	TemporaryEffectComponent->StunCharacter(TweakeableData->StunDuration);

	if (TweakeableData->StunDuration > 0.0f)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, TemporaryEffectComponent]()
											   {
												   TemporaryEffectComponent->SlowCharacter(
													   TweakeableData->MinimumRatioOfSpeed,
													   TweakeableData->DurationSlowTrap);
												   //Destroy();
											   },
											   TweakeableData->StunDuration, false);
	}
	else
	{
		TemporaryEffectComponent->SlowCharacter(
			TweakeableData->MinimumRatioOfSpeed,
			TweakeableData->DurationSlowTrap);
		//Destroy();
	}
	Super::ActivateTrap(OtherActor);
}


void AIncapacitedTrap::UseItem()
{
	Super::UseItem();
}
