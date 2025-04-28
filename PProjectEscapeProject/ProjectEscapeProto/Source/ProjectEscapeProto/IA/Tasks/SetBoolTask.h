// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetBoolTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API USetBoolTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USetBoolTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BoolKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool BoolValue;
};
