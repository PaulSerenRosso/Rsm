// Fill out your copyright notice in the Description page of Project Settings.


#include "SyncRoomPatrol.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"

USyncRoomPatrol::USyncRoomPatrol()
{
	NodeName = TEXT("Sync Room Patrol");
}

EBTNodeResult::Type USyncRoomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AHearingNeighbourController* HearingNeighbourController = Cast<AHearingNeighbourController>(OwnerComp.GetAIOwner());
	if (HearingNeighbourController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = HearingNeighbourController->GetPawn();
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	BlackboardComp->SetValueAsInt(GetCurrentRoomKey.SelectedKeyName, static_cast<uint8>(HearingNeighbourController->CurrentRoom));
	if (static_cast<uint8>(HearingNeighbourController->CurrentRoom) == BlackboardComp->GetValueAsInt(GetCurrentRoomKey.SelectedKeyName))
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
