// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectEscapeProto/ActorComponents/TemporaryEffects/TemporaryEffect.h"
#include <Net/UnrealNetwork.h>

#include "EntityController.h"
#include "Components/CapsuleComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/InteractActionComponent.h"

class UBlackboardComponent;
// Sets default values
AAICharacter::AAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	TemporaryEffectComponent = CreateDefaultSubobject<UTemporaryEffect>(TEXT("TemporaryEffect"));
	TemporaryEffectComponent->SetupAttachment(RootComponent);
}

void AAICharacter::Init(USpawnEntityData* SpawnEntityData)
{
	if(HasAuthority())
	{
		AEntityController* MYAIController = Cast<AEntityController>(GetController());
		MYAIController->Init(SpawnEntityData);
	}
}

void AAICharacter::Multicast_DeactivateCollider_Implementation()
{
	UCapsuleComponent* Collider = GetCapsuleComponent();
	Collider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_Interactable, ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	
	
}



// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	Movement = GetCharacterMovement();
	if (!Movement.IsValid()) return;
	BasicMaxWalkSpeed = Movement->MaxWalkSpeed;
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentWaitTimer > 0.0f)
		SetWaitingTime(CurrentWaitTimer - DeltaTime);

	if (TimerIncreaseSpeed.Num() > 0)
	{
		FGenericFloatGoal Goal = TimerIncreaseSpeed[0];
		float NewIncrease = Goal.StepTime * DeltaTime;

		if (NewIncrease > 0.0f && Goal.Current + NewIncrease >= Goal.Goal)
		{
			NewIncrease = Goal.Goal - Goal.Current;
			if (Goal.Current + NewIncrease > Goal.Goal)
				NewIncrease = 0.0f;
		}
		else if (NewIncrease < 0.0f && Goal.Current + NewIncrease <= Goal.Goal)
		{
			NewIncrease = Goal.Goal - Goal.Current;
			if (Goal.Current + NewIncrease < Goal.Goal)
				NewIncrease = 0.0f;
		}
			TimerIncreaseSpeed[0].Current += NewIncrease;

		TimerIncreaseSpeed[0].Duration -= DeltaTime;
		if (TimerIncreaseSpeed[0].Duration <= 0.0f)
		{
			TimerIncreaseSpeed.RemoveAt(0);
			UE_LOG(LogTemp, Warning, TEXT("END OF TIMER"));
		}
		IncreaseWalkSpeed(NewIncrease);
	}
}

void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter::SetWaitingTime(float Timer)
{
	if (!BlackboardComp)
	{
		AAIController* MYAIController = Cast<AAIController>(GetController());
		if (!MYAIController) return;
		BlackboardComp = MYAIController->GetBlackboardComponent();
	}
	if (BlackboardComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlackboardComp is null"));
		return;
	}

	BlackboardComp->SetValueAsFloat(WaitingTimeKey, Timer);
	CurrentWaitTimer = Timer;
	TemporaryEffectComponent->StunCharacter(Timer);
}

void AAICharacter::IncreaseWalkSpeed(float Speed)
{
	if (Speed == 0.0f) return;
	BasicMaxWalkSpeed += Speed;
	if (!Movement.IsValid())
		Movement = GetCharacterMovement();

	if (!Movement.IsValid() || Movement->MaxWalkSpeed <= 0.0f) return;

	Movement->MaxWalkSpeed = BasicMaxWalkSpeed * CurrentSlowRatio;
	//UE_LOG(LogTemp, Warning, TEXT("Speed increased to %f"), Movement->MaxWalkSpeed);
}

void AAICharacter::IncreaseWalkSpeedOnStack(float Speed, float StepTime, float Duration)
{
	FGenericFloatGoal Goal;
	Goal.Goal = Speed;
	Goal.Current = 0.0f;
	Goal.StepTime = Speed / StepTime;
	Goal.Duration = Duration;
	TimerIncreaseSpeed.Add(Goal);
}

void AAICharacter::OnKillPlayer()
{
	IsKillingPlayer.Broadcast();
}

void AAICharacter::OnHearSound()
{
	IsHearingBiggestSound.Broadcast();
}

void AAICharacter::SwitchToHumanMonsterMode(bool IsMonster)
{
	AreInMonsterMode = IsMonster;
	IsTransforming.Broadcast(IsMonster);
}

void AAICharacter::ApplySlow_Implementation(float Ratio)
{
	CurrentSlowRatio = Ratio;
	Movement = GetCharacterMovement();
	if (!Movement.IsValid()) return;
	Movement->MaxWalkSpeed = BasicMaxWalkSpeed * CurrentSlowRatio;
}

void AAICharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAICharacter, AreInMonsterMode);
}
