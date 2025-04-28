// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestData.generated.h"

struct FActorSpawnInfo;
class UQuest;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UQuestData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TSubclassOf<UQuest> QuestType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FString QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FString QuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FActorSpawnInfo> QuestActors;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	bool IsAQuantifiedQuest;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	bool IsSoloQuest;

};
