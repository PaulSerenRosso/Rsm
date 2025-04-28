// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ElevatorManagerData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UElevatorManagerData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float DelayOpeningAtEndPhase = 10;
	UPROPERTY(EditAnywhere)
	float StartGameElevatorTime = 5;
	UPROPERTY(EditAnywhere)
	float EndGameElevatorTime = 5 ;
	
};
