// Fill out your copyright notice in the Description page of Project Settings.


#include "IsOnDistanceTask.h"

#include "BehaviorTree/BlackboardComponent.h"

UIsOnDistanceTask::UIsOnDistanceTask()
{
	
}

EBTNodeResult::Type UIsOnDistanceTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	FVector FromDistance = BlackboardComp->GetValueAsVector(FromDistanceKey.SelectedKeyName);
	FVector ToDistance = BlackboardComp->GetValueAsVector(ToDistanceKey.SelectedKeyName);
	float Distance = FVector::Distance(FromDistance, ToDistance);
	if (Distance <= DistanceMax)
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
