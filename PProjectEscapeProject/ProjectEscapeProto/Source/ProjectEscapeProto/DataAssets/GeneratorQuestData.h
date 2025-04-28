// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestData.h"
#include "Engine/DataAsset.h"
#include "GeneratorQuestData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UGeneratorQuestData : public UQuestData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeneratorQuestData")
	int ScoreAmountByHit;
};
