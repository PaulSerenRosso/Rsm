// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JeremiePhase.generated.h"

class AHearingNeighbourController;

UCLASS(Blueprintable, BlueprintType)
class PROJECTESCAPEPROTO_API UJeremiePhase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPhaseStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPhaseEnd();
	void SetOwnerController(AHearingNeighbourController* Controller);

public:
	UPROPERTY(EditAnywhere)
	float TimerBeforeTrigger = 600.0f;
	UPROPERTY(BlueprintReadOnly)
	AHearingNeighbourController* OwnerController = nullptr;
};
