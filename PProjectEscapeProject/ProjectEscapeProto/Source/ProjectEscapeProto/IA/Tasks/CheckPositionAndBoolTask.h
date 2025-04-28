// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CheckPositionAndBoolTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UCheckPositionAndBoolTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCheckPositionAndBoolTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LocationKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float AcceptanceRadius = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ConditionBoolKey;
	
};
