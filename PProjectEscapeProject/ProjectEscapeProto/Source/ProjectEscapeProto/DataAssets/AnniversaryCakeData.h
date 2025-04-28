// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AnniversaryCakeData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UAnniversaryCakeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnniversaryCake")
	float MoveSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnniversaryCake")
	int ScoreWhenArrived = 100;
};
