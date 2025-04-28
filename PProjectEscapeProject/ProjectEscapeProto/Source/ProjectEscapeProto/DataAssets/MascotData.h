// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MascotData.generated.h"


class UMascotMetricsAnalyzer;
class UMascotSensor;
class UMascotTagAction;

UCLASS(Blueprintable, BlueprintType)
class PROJECTESCAPEPROTO_API UMascotData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UMascotSensor>> MascotteSensors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UMascotMetricsAnalyzer>> AllMetricsAnalyzer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UMascotTagAction>> AllTagActions;
};
