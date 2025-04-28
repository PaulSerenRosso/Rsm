// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPlayerIsInSightTask.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"

UCheckPlayerIsInSightTask::UCheckPlayerIsInSightTask()
{
	
}

EBTNodeResult::Type UCheckPlayerIsInSightTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (AIController->CanSeePlayer())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("Player in sight"));
		BlackboardComp->SetValueAsBool(PlayerInSightKey.SelectedKeyName, true);
		BlackboardComp->SetValueAsObject(PlayerObjectKey.SelectedKeyName, Cast<AActor> (AIController->GetPlayer()));
		
		AIController->SwapState(EEntityState::Hunt);
		return EBTNodeResult::Succeeded;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("Player not in sight"));
	BlackboardComp->SetValueAsBool(PlayerInSightKey.SelectedKeyName, false);
	BlackboardComp->SetValueAsObject(PlayerObjectKey.SelectedKeyName, nullptr);
	return EBTNodeResult::Failed;
}
