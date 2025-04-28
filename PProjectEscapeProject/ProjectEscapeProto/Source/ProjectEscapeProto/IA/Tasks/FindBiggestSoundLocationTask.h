// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindBiggestSoundLocationTask.generated.h"

class UNavigationSystemV1;
class UHearingNeighbourData;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UFindBiggestSoundLocationTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UFindBiggestSoundLocationTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SoundLocationKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SoundActorKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LastHearingSoundStrengthKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UHearingNeighbourData* NeighbourEntityData;
	float BestSoundStrength = 0.0f;
	TWeakObjectPtr<UNavigationSystemV1> NavSystem;
};