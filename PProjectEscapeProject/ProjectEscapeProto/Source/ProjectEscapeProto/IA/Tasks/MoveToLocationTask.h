// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MoveToLocationTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UMoveToLocationTask : public UBTTaskNode
{
	GENERATED_BODY()
	
	UMoveToLocationTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LocationKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float AcceptanceRadius;
};
