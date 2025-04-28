// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PushData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UPushData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float PushCooldown = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float PushForce = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float PushDuration = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float RangeToPush = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float PushStaminaCost = 100.f;

};
