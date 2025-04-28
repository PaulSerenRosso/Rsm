// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestData.h"
#include "RegroupQuestData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API URegroupQuestData : public UQuestData
{
	GENERATED_BODY()
public:
	int ScoreWhenQuestSucceeded = 10;
};
