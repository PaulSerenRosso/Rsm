// Fill out your copyright notice in the Description page of Project Settings.


#include "TemporaryEffect.h"

#include "PushEffectComponent.h"
#include "SlowEffectComponent.h"
#include "StunEffectComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionStateComponent.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/IA/AICharacter.h"

// Sets default values for this component's properties
UTemporaryEffect::UTemporaryEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	StunEffectComponent = CreateDefaultSubobject<UStunEffectComponent>(TEXT("StunEffectComponent"));
	SlowEffectComponent = CreateDefaultSubobject<USlowEffectComponent>(TEXT("SlowEffectComponent"));
	PushEffectComponent = CreateDefaultSubobject<UPushEffectComponent>(TEXT("PushEffectComponent"));
}
// Called when the game starts
void UTemporaryEffect::BeginPlay()
{
	Super::BeginPlay();
	// StunEffectComponent->SetupAttachment(this);
	// SlowEffectComponent->SetupAttachment(this);
	// PushEffectComponent->SetupAttachment(this);
}

void UTemporaryEffect::StunCharacter(float StunTime)
{
	StunEffectComponent->StunCharacter(StunTime);
}

void UTemporaryEffect::PushCharacter(float PushTime, FVector Direction, float Force)
{
	//StunCharacter(PushTime);
	PushEffectComponent->PushCharacter(PushTime, Direction, Force);
}

void UTemporaryEffect::SlowCharacter(float Ratio, float Duration)
{
	SlowEffectComponent->SlowCharacter(Ratio, Duration);
}

void UTemporaryEffect::RemoveStun()
{
	StunEffectComponent->RemoveStun();
}

void UTemporaryEffect::RemoveSlow()
{
	SlowEffectComponent->RemoveSlow();
}

bool UTemporaryEffect::IsAlreadyPushed() const
{
	return PushEffectComponent->IsAlreadyPushed();
}

// Called every frame
void UTemporaryEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
