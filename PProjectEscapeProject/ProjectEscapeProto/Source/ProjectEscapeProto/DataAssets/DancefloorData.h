// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DancefloorData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UDancefloorData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DelayTickBetweenPoints = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PointByTick = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NbPlayersNeedToStart = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeBeforeFinish = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LoudnessNoiseEmitted = 500;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxRangeNoiseEmitted = 1000;
	
};
