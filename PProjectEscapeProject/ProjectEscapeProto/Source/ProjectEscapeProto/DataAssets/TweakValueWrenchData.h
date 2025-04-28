// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TweakValueWrenchData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UTweakValueWrenchData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToBeReusable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Durability = 5;
};
