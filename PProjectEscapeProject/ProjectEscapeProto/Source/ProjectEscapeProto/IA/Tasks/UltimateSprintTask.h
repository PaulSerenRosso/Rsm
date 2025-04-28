// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UltimateSprintTask.generated.h"

class UHearingNeighbourData;
class AAICharacter;
class AHearingNeighbourController;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UUltimateSprintTask : public UObject
{
	GENERATED_BODY()

public:
	UUltimateSprintTask();
	void Execute(AHearingNeighbourController* AIController, AAICharacter* AICharacter, UHearingNeighbourData* NeighbourEntityData);

public:
	float Cooldown = 0.0f;
};
