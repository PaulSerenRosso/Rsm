// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HearingNeighbourData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UHearingNeighbourData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float HearingRange = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float SightRadius = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float LoseSightRadius = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float PeripheralVisionAngleDegrees = 180.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float DurationAfterOutSighAndBeforeListenAgain = 180.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float TimerBeforeCanSwitchToTarget = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hunt")
	float ModifierSightOfHunt = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hunt")
	float ModifierHeardOfHunt = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hunt")
	float RatioModifierSightOfHuntPerSecond = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hunt")
	float RatioModifierHearingOfHuntPerSecond = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chase")
	float ModifierSightOfChase = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chase")
	float ModifierHeardOfChase = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chase")
	float RatioModifierSightOfChasePerSecond = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chase")
	float RatioModifierHearingOfChasePerSecond = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	float ModifierSightOfPatrol = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	float ModifierHeardOfPatrol = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	float RatioModifierSightOfPatrolPerSecond = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	float RatioModifierHearingOfPatrolPerSecond = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint")
	float UltimateSprintCooldown = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint | Increase")
	float UltimateSprintIncreaseSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint | Increase")
	float UltimateSprintIncreaseStepTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint | Increase")
	float UltimateSprintIncreaseDuration = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint | Decrease")
	float UltimateSprintDecreaseStepTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint | Decrease")
	float UltimateSprintDecreaseDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint | Exhaust")
	float UltimateSprintExhaustSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint | Exhaust")
	float UltimateSprintExhaustStepTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint | Exhaust")
	float UltimateSprintExhaustDuration = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate Sprint | BackToNormal")
	float UltimateSprintBackToNormalStepTime = 1.0f;

};
