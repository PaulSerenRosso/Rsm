// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestManagerData.generated.h"

class UQuestData;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UQuestManagerData : public UDataAsset
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<UQuestData*> MainQuests;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<UQuestData*> SecondaryQuest;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	float TimerBeforeLaunchSecondaryQuest = 300;
};
