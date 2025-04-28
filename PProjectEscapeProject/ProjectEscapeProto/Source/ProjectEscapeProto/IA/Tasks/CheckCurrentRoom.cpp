// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckCurrentRoom.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"

UCheckCurrentRoom::UCheckCurrentRoom()
{
	NodeName = TEXT("Check Current Room");
}

EBTNodeResult::Type UCheckCurrentRoom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	if (static_cast<uint8>(HearingNeighbourController->CurrentRoom) != BlackboardComp->GetValueAsInt(GetCurrentRoomKey.SelectedKeyName))
	{
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}
