// Fill out your copyright notice in the Description page of Project Settings.


#include "KillPlayerTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/IA/AICharacter.h"


UKillPlayerTask::UKillPlayerTask()
{
	NodeName = "Kill Player";
}

EBTNodeResult::Type UKillPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
	APrisoner* Player = Cast<APrisoner>(BlackboardComp->GetValueAsObject(PlayerTargetKey.SelectedKeyName));
	if (!Player)
	{
		return EBTNodeResult::Failed;
	}

	AAICharacter* AICharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	//if (AICharacter)
		//AICharacter->OnKillPlayer();
	//Player->TryDiePlayer(AICharacter);

	return EBTNodeResult::Succeeded;
}
