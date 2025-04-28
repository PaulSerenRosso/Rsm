// Fill out your copyright notice in the Description page of Project Settings.


#include "DespawnEntityTask.h"

#include "AIController.h"
#include "ProjectEscapeProto/OnlineGameMode.h"

EBTNodeResult::Type UDespawnEntityTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AOnlineGameMode* OnlineGameMode = Cast<AOnlineGameMode>(OwnerComp.GetAIOwner()->GetWorld()->GetAuthGameMode());
	if(OnlineGameMode)
	{
		OnlineGameMode->AskDestroyEntity(Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetCharacter()));
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;

}
