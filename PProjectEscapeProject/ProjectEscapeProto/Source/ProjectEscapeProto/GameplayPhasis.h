// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Utilities/EnumsUtilities.h"
#include "GameplayPhasis.generated.h"

class USpawnEntityData;
class UQuestManagerData;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTESCAPEPROTO_API UGameplayPhasis : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UQuestManagerData* QuestManagerData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float PhasisDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<ERoomType> RoomsOpened;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<ERoomType> RoomsClosed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<USpawnEntityData*> EntitiesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FText PhasisStartDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FText PhasisEndDescription;
};
