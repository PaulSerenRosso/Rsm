// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "IsNearOfObjectDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UIsNearOfObjectDecorator : public UBTDecorator
{
	GENERATED_BODY()

	UIsNearOfObjectDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FBlackboardKeySelector ObjectToCheckKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AcceptanceRadius;
	
};
