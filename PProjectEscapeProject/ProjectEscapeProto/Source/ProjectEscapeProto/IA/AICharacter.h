// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "ProjectEscapeProto/Utilities/StructUtilities.h"
#include "AICharacter.generated.h"

class USpawnEntityData;
class UTemporaryEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBlankDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTransformDelegate, bool, InMonsterMode);

UCLASS()
class PROJECTESCAPEPROTO_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();
	
	UFUNCTION(Server, Reliable)
	void ApplySlow(float Ratio);
	UFUNCTION(BlueprintCallable)
	void IncreaseWalkSpeedOnStack(float Speed, float StepTime = 0.0f, float Duration = 0.0f);
	void OnKillPlayer();
	void OnHearSound();
	void SwitchToHumanMonsterMode(bool IsMonster);
	bool IsInMonsterMode() const { return AreInMonsterMode; }
	void Init(USpawnEntityData* Get);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DeactivateCollider();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void SetWaitingTime(float Timer);
	UFUNCTION(BlueprintCallable)
	void IncreaseWalkSpeed(float Speed);
	
	
public:
	float BasicMaxWalkSpeed = 0.0f;
	TWeakObjectPtr<UTemporaryEffect> TemporaryEffectComponent = nullptr;

protected:
	float CurrentSlowRatio = 1.0f;
	UPROPERTY(EditAnywhere)
	FName WaitingTimeKey = "WaitingTime";
	float CurrentWaitTimer = 0.0f;
	UBlackboardComponent* BlackboardComp = nullptr;
	TArray<FGenericFloatGoal> TimerIncreaseSpeed;
	TWeakObjectPtr<UCharacterMovementComponent> Movement = nullptr;
	
	UPROPERTY(BlueprintAssignable)
	FOnBlankDelegate IsKillingPlayer;
	UPROPERTY(BlueprintAssignable)
	FOnBlankDelegate IsHearingBiggestSound;
	UPROPERTY(BlueprintAssignable)
	FTransformDelegate IsTransforming;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Replicated)
	bool AreInMonsterMode = false;
	
};
