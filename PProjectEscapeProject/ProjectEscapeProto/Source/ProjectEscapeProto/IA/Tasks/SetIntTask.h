// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetIntTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API USetIntTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USetIntTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IntKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	int IntValue;
};
