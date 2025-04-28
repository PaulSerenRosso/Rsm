// Fill out your copyright notice in the Description page of Project Settings.


#include "AssignNextPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"

UAssignNextPatrolPoint::UAssignNextPatrolPoint()
{
}

EBTNodeResult::Type UAssignNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (HearingNeighbourController->GetPatrolPointsCount() == 0)
	{
		return EBTNodeResult::Failed;
	}

	FVector NextLocation = HearingNeighbourController->GetFirstPatrolLocation();

	BlackboardComp->SetValueAsVector(NextPatrolPointKey.SelectedKeyName, NextLocation);

	HearingNeighbourController->SwapState(EEntityState::Patrol);
	return EBTNodeResult::Succeeded;
}
