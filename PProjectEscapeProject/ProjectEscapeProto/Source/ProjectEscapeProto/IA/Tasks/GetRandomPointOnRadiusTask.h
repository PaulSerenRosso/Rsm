// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetRandomPointOnRadiusTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UGetRandomPointOnRadiusTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UGetRandomPointOnRadiusTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	bool GetValidRandomPoint(FVector& OutLocation, const FVector& Origin, AActor* Owner) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Radius;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector RandomPointKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LocationSourceKey;
};
