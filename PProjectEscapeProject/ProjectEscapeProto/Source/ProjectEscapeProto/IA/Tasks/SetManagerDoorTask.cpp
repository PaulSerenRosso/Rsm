// Fill out your copyright notice in the Description page of Project Settings.


#include "SetManagerDoorTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/MapInfoSubsystem.h"

#include "ProjectEscapeProto/DataAssets/SpawnJeremieData.h"
#include "ProjectEscapeProto/Managers/MapInfoManager.h"
#include "ProjectEscapeProto/Objects/LDElements/ManagerDoor.h"

EBTNodeResult::Type USetManagerDoorTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	USpawnJeremieData* SpawnEntityData = Cast<USpawnJeremieData>(BlackboardComp->GetValueAsObject(SpawnEntityDataKeySelector.SelectedKeyName));
	if (SpawnEntityData)
	{
		UMapInfoSubsystem* MapInfoSubsystem = OwnerComp.GetAIOwner()->GetGameInstance()->GetSubsystem<UMapInfoSubsystem>();
		if(MapInfoSubsystem)
		{
			if(MapInfoSubsystem->MapInfoManager != nullptr)
			{
				TArray<AActor*> ManagerDoorArray = MapInfoSubsystem->MapInfoManager.Get()->GetActorWithTag(SpawnEntityData->TagManagerDoor);
				if(ManagerDoorArray.Num() > 0)
				{
					AManagerDoor* ManagerDoor =	Cast<AManagerDoor>(ManagerDoorArray[0]);
					if(ManagerDoor)
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsObject(DoorManagerKeySelector.SelectedKeyName, ManagerDoor);
						return EBTNodeResult::Succeeded;
					}
				}
			}			
		}
	}
	return EBTNodeResult::Failed;
	
}
