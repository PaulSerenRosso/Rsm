// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateSprintTask.h"
#include "AIController.h"
#include "ProjectEscapeProto/DataAssets/HearingNeighbourData.h"
#include "ProjectEscapeProto/IA/AICharacter.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"

UUltimateSprintTask::UUltimateSprintTask()
{
	
}

void UUltimateSprintTask::Execute(AHearingNeighbourController* AIController, AAICharacter* AICharacter,
	UHearingNeighbourData* NeighbourEntityData)
{

	Cooldown = NeighbourEntityData->UltimateSprintCooldown;
	
	AICharacter->IncreaseWalkSpeedOnStack(NeighbourEntityData->UltimateSprintIncreaseSpeed,
	                                      NeighbourEntityData->UltimateSprintIncreaseStepTime,
	                                      NeighbourEntityData->UltimateSprintIncreaseDuration);
	
	AICharacter->IncreaseWalkSpeedOnStack(-NeighbourEntityData->UltimateSprintIncreaseSpeed,
										  NeighbourEntityData->UltimateSprintDecreaseStepTime,
										  NeighbourEntityData->UltimateSprintDecreaseDuration);
	
	AICharacter->IncreaseWalkSpeedOnStack(-NeighbourEntityData->UltimateSprintExhaustSpeed,
									  NeighbourEntityData->UltimateSprintExhaustStepTime,
									  NeighbourEntityData->UltimateSprintExhaustDuration);
	
	AICharacter->IncreaseWalkSpeedOnStack(NeighbourEntityData->UltimateSprintExhaustSpeed,
								  NeighbourEntityData->UltimateSprintBackToNormalStepTime,
								  NeighbourEntityData->UltimateSprintBackToNormalStepTime);
}
