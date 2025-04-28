// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsAtLocationPoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UIsAtLocationPoint : public UBTDecorator
{
	GENERATED_BODY()

public:
	UIsAtLocationPoint();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LocationKey;
	UPROPERTY(EditAnywhere,	Category = "Blackboard")
	float AcceptanceRadius = 150;
};
