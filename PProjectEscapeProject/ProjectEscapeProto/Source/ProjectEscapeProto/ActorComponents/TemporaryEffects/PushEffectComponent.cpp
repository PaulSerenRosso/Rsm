// Fill out your copyright notice in the Description page of Project Settings.


#include "PushEffectComponent.h"
#include "GameFramework/Character.h"
#include "ProjectEscapeProto/ActorComponents/InventorySystem.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"

UPushEffectComponent::UPushEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPushEffectComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UPushEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPushEffectComponent::PushCharacter(float PushTime, FVector Direction, float Force)
{
	ArePushed = true;
	
	FVector NormalizedVector = Direction.GetSafeNormal();
	NormalizedVector.Z = 0.f;
	APrisoner* Prisoner = Cast<APrisoner>(GetOwner());
	if (Prisoner)
	{
		Prisoner->Inventory->TryRemoveItem(Prisoner->Inventory->FirstItemSlot, Prisoner, true, 0);
     	Prisoner->LaunchCharacter(NormalizedVector * Force, true, false);
		
	}


	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		ArePushed = false;
	}, PushTime, false);
}

bool UPushEffectComponent::IsAlreadyPushed() const
{
	return ArePushed;
}
