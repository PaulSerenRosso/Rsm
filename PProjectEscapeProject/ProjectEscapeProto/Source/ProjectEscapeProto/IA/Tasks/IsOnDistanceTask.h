// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "IsOnDistanceTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UIsOnDistanceTask : public UBTTaskNode
{
	GENERATED_BODY()

	UIsOnDistanceTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector FromDistanceKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector ToDistanceKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float DistanceMax;
	
};
