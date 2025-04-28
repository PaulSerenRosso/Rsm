// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DeactivateCharacterColliderTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UDeactivateCharacterColliderTask : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:


};
