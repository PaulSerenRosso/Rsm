// Fill out your copyright notice in the Description page of Project Settings.


#include "SetBoolTask.h"

#include "BehaviorTree/BlackboardComponent.h"

USetBoolTask::USetBoolTask()
{
	NodeName = "Set Bool Task";
}

EBTNodeResult::Type USetBoolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	BlackboardComp->SetValueAsBool(BoolKey.SelectedKeyName, BoolValue);
	return EBTNodeResult::Succeeded;
}
