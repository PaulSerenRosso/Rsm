// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VivoxCore.h"
#include "ProximityChatData.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UProximityChatData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProximityChat")
	int AudibleDistance = 2700;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProximityChat")
	int ConversationalDistance = 90;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProximityChat")
	double AudioFadeIntensityByDistance = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProximityChat")
	float RatioNoiseEmittedByVoiceActivity = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProximityChat")
	EAudioFadeModel AudioFadeModel = EAudioFadeModel::InverseByDistance;
};
