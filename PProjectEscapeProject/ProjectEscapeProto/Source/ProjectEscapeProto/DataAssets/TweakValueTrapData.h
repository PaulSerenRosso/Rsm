// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TweakValueTrapData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UTweakValueTrapData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float MaxDistanceActivateTrap = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float TimerBeforeActivateTrap = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool CanBeActivateMultipleTimes = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float DelayBeforeReusingTrap = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool NeedToBeDestroy = false;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stun")
	float StunDuration = 0.5f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slow")
	float MinimumRatioOfSpeed = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slow")
	float DurationSlowTrap = 5.0f;
};
