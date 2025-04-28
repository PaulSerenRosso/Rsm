// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetManagerDoorTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API USetManagerDoorTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SpawnEntityDataKeySelector;	

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DoorManagerKeySelector;
};
