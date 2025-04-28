// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPositionAndBoolTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCheckPositionAndBoolTask::UCheckPositionAndBoolTask()
{
	NodeName = TEXT("Check Position And Bool");
}

EBTNodeResult::Type UCheckPositionAndBoolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector CurrentPawnLocation = AIPawn->GetActorLocation();
	CurrentPawnLocation.Z -= AIPawn->GetSimpleCollisionHalfHeight();
	float Distance = FVector::Dist(CurrentPawnLocation, BlackboardComp->GetValueAsVector(LocationKey.SelectedKeyName));
	bool ConditionBool = BlackboardComp->GetValueAsBool(ConditionBoolKey.SelectedKeyName);
	if ((Distance <= AcceptanceRadius) || !ConditionBool )
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
