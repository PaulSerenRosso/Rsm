// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnEntityData.generated.h"

class AAICharacter;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API USpawnEntityData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float TimerBeforeSpawnEntity = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	FString TagLocationOfSpawning = "EntitySpawn";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AAICharacter> EntityToSpawn;
};
