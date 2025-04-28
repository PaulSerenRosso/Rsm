// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWithManagerDoorTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/AHearingNeighbourCharacter.h"
#include "ProjectEscapeProto/Objects/LDElements/ManagerDoor.h"

EBTNodeResult::Type UInteractWithManagerDoorTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AManagerDoor* ManagerDoor = Cast<AManagerDoor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(DoorManagerKeySelector.SelectedKeyName));
	if (ManagerDoor)
	{
		AAHearingNeighbourCharacter* HearingNeighbourCharacter = Cast<AAHearingNeighbourCharacter>(OwnerComp.GetAIOwner()->GetPawn());
		if(HearingNeighbourCharacter)
		{
			HearingNeighbourCharacter->BP_OnInteractWithManagerDoor(ManagerDoor, IsExit);
		}
	
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	
}
