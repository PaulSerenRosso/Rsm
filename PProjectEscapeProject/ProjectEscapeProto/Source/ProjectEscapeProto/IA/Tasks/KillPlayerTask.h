// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "KillPlayerTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UKillPlayerTask : public UBTTaskNode
{
	GENERATED_BODY()

	UKillPlayerTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector PlayerTargetKey;
};
