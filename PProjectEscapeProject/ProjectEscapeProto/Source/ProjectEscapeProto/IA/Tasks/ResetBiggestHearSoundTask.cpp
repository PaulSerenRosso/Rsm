// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetBiggestHearSoundTask.h"

#include "BehaviorTree/BlackboardComponent.h"

UResetBiggestHearSoundTask::UResetBiggestHearSoundTask()
{
	NodeName = "Reset Biggest Hear Sound";
}

EBTNodeResult::Type UResetBiggestHearSoundTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
	BlackboardComp->SetValueAsFloat(LastHearingSoundStrengthKey.SelectedKeyName, 0.0f);
	return EBTNodeResult::Succeeded;
}
