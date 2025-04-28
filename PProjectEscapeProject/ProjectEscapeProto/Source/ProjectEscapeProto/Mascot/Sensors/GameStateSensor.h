// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/Mascot/MascotSensor.h"
#include "GameStateSensor.generated.h"

class UQuest;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UGameStateSensor : public UMascotSensor
{
	GENERATED_BODY()

protected:
	virtual void InitSensor(UWorld* WorldContext, AMascot* Mascot, APrisonerController *PC) override;
	virtual void UpdateMetrics() override;
	UFUNCTION()
	void StartQuest(UQuest* Quest);
	UFUNCTION()
	void QuestsTerminated(bool IsSuccess);

};
