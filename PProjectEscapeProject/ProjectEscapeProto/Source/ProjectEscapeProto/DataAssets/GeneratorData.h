// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeneratorData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UGeneratorData : public UDataAsset
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generator")
	// 	int ScoreAmount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generator")
		int MaxStepsToRepare;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generator")
	float DistanceBeforeSeeWidget;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Noise")
	float LoudnessNoiseEmitted;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Noise")
	float MaxRangeNoiseEmittec;
};
