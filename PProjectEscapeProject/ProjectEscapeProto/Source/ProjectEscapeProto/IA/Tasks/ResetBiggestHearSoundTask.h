// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ResetBiggestHearSoundTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UResetBiggestHearSoundTask : public UBTTaskNode
{
	GENERATED_BODY()

	public:
		UResetBiggestHearSoundTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LastHearingSoundStrengthKey;
};
