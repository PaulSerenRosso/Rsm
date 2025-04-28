// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CheckPlayerIsInSightTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UCheckPlayerIsInSightTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCheckPlayerIsInSightTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector PlayerInSightKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector PlayerObjectKey;
};
