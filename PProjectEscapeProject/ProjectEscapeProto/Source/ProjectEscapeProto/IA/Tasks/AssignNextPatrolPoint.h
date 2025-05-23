// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AssignNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UAssignNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UAssignNextPatrolPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector NextPatrolPointKey;
	
};
