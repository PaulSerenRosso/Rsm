// Fill out your copyright notice in the Description page of Project Settings.


#include "ComputePatrolPoints.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"


UComputePatrolPoints::UComputePatrolPoints()
{
	
}

EBTNodeResult::Type UComputePatrolPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AHearingNeighbourController* HearingNeighbourController = Cast<AHearingNeighbourController>(OwnerComp.GetAIOwner());
	if (HearingNeighbourController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("HearingNeighbourController is null"));
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = HearingNeighbourController->GetPawn();
	if (AIPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIPawn is null"));
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlackboardComp is null"));
		return EBTNodeResult::Failed;
	}

	char NumPatrolPoints = HearingNeighbourController->SetCurrentPatrolLocations(HearingNeighbourController->CurrentRoom);
	BlackboardComp->SetValueAsInt(RemainingRoomsKey.SelectedKeyName, NumPatrolPoints);
	if (NumPatrolPoints == 0)
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}
