// Fill out your copyright notice in the Description page of Project Settings.


#include "SetPositionOfLastSeenSightTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"

USetPositionOfLastSeenSightTask::USetPositionOfLastSeenSightTask()
{
}

EBTNodeResult::Type USetPositionOfLastSeenSightTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AHearingNeighbourController* AIController = Cast<AHearingNeighbourController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Player = Cast<AActor>(BlackboardComp->GetValueAsObject(PlayerTargetKey.SelectedKeyName));
	if (!Player)
	{
		return EBTNodeResult::Failed;
	}

	FVector LastSeenSightLocation = Player->GetActorLocation();
	BlackboardComp->SetValueAsVector(LocationToGoKey.SelectedKeyName, LastSeenSightLocation);
	BlackboardComp->SetValueAsVector(LastSighPositionKey.SelectedKeyName, LastSeenSightLocation);
	return EBTNodeResult::Succeeded;
}
