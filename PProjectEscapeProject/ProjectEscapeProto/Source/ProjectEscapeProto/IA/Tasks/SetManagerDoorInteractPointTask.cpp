// Fill out your copyright notice in the Description page of Project Settings.


#include "SetManagerDoorInteractPointTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/Objects/LDElements/ManagerDoor.h"

EBTNodeResult::Type USetManagerDoorInteractPointTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AManagerDoor* ManagerDoor = Cast<AManagerDoor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(DoorManagerKeySelector.SelectedKeyName));
	if (ManagerDoor)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(InteractPointSelector.SelectedKeyName,ManagerDoor->ExitInteractPoint->GetComponentLocation());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
	
}
