// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/Mascot/MascotSensor.h"
#include "ElevatorSensor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UElevatorSensor : public UMascotSensor
{
	GENERATED_BODY()
	virtual void InitSensor(UWorld* WorldContext, AMascot* Mascot, APrisonerController * PC) override;
	virtual void UpdateMetrics() override;

protected:
	UFUNCTION()
	void OnPrisonerEntered(int Index);
	UFUNCTION()
	void OnElevatorClose();

protected:
	int NbEnteredPrisonner = 0;
};
