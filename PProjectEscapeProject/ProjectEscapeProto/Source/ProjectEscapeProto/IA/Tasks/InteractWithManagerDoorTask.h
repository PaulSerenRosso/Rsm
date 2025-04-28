// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "InteractWithManagerDoorTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UInteractWithManagerDoorTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DoorManagerKeySelector;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool IsExit = false;
};
