// Fill out your copyright notice in the Description page of Project Settings.


#include "DecreaseIntTask.h"

#include "BehaviorTree/BlackboardComponent.h"

UDecreaseIntTask::UDecreaseIntTask()
{
}

EBTNodeResult::Type UDecreaseIntTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	int Value = OwnerComp.GetBlackboardComponent()->GetValueAsInt(IntKey.SelectedKeyName);
	Value--;
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(IntKey.SelectedKeyName, Value);
	return EBTNodeResult::Succeeded;
}
