// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SwitchStateEntityTask.generated.h"

enum class EEntityState : uint8;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API USwitchStateEntityTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	EEntityState StateKeySelector;
};
