// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnEntityData.h"
#include "SpawnJeremieData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API USpawnJeremieData : public USpawnEntityData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	FString TagManagerDoor = "Door";
};
