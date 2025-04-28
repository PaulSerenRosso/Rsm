// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TweakeableValueCharacterData.generated.h"

class AScoreDrop;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UTweakeableValueCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float BaseForwardMoveSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float BaseBackwardMoveSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float BaseSideMoveSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float BaseNoiseEmitted = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float WalkForwardMoveSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float WalkBackwardMoveSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float WalkSideMoveSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float WalkNoiseEmitted = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float WalkSoundMultiplier = 0.75f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	USoundAttenuation* WalkSoundAttenuation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float SprintForwardMoveSpeed = 400.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float SprintBackwardMoveSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float SprintSideMoveSpeed = 1000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float SprintNoiseEmitted = 1000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float SprintInitialBurnCost = 3.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float SprintBurnCostSpeed = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float SprintSoundMultiplier = 1.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	USoundAttenuation* SprintSoundAttenuation;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float CrouchedMoveSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float PlayerSizeCrouched = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float JumpHeight = 400.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement")
	float JumpNoiseEmitted = 400.0f;
	//==============================Stamina=========================================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Stamina")
	float StaminaMax = 175.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Stamina")
	float StaminaRecoverySpeedInPercent = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Stamina")
	float StaminaDelayBeforeRecovery = 2.0f;
	
	//==============================INVENTORY=========================================


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Inventory")
	float SuctionSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Inventory")
	float DropDistance = 200.0f;

	//==============================Stamina=========================================
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Interaction")
	float InteractRange = 200.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Rotation")
	float CameraRotationPitchSpeed = 50.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Rotation")
	float CameraRotationPitchMax = 10.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Rotation")
	float CameraRotationPitchMin = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Respawn")
	float TimerBeforeRespawn = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Respawn")
	float PercentageCostToRespawn = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Respawn")
	float FlatCostToRespawn = 50.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Respawn")
	TSubclassOf<AScoreDrop> ScoreDropActor;
};
