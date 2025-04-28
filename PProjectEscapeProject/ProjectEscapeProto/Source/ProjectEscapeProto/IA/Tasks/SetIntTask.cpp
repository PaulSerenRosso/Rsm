// Fill out your copyright notice in the Description page of Project Settings.


#include "SetIntTask.h"

#include "BehaviorTree/BlackboardComponent.h"

USetIntTask::USetIntTask()
{
}

EBTNodeResult::Type USetIntTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;
	
	BlackboardComp->SetValueAsInt(IntKey.SelectedKeyName, IntValue);
	return EBTNodeResult::Succeeded;
}
