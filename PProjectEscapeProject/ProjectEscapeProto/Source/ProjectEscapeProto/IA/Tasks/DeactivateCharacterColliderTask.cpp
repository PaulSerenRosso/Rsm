// Fill out your copyright notice in the Description page of Project Settings.


#include "DeactivateCharacterColliderTask.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/InteractActionComponent.h"
#include "ProjectEscapeProto/IA/AICharacter.h"

EBTNodeResult::Type UDeactivateCharacterColliderTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAICharacter* Character = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetCharacter());
	if (Character)
	{
		Character->Multicast_DeactivateCollider();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
