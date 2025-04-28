// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchStateEntityTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"

EBTNodeResult::Type USwitchStateEntityTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AHearingNeighbourController* HearingNeighbourController = Cast<AHearingNeighbourController>(OwnerComp.GetAIOwner());
	if (HearingNeighbourController)
	{
		HearingNeighbourController->SwapState(StateKeySelector);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
