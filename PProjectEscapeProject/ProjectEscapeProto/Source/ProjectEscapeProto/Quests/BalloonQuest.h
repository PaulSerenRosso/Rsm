// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "BalloonQuest.generated.h"

class UBalloonData;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UBalloonQuest : public UQuest
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void BalloonFinished(bool WithSuccess);
	virtual void StartQuest(UWorld* World) override;
	virtual void OnQuestFail() override;
	virtual void OnQuestSuccess() override;
	virtual FString GetQuestProgress() override;

protected:
	int CurrentStep = 0;
	UPROPERTY(EditAnywhere)
	int MaxStep = 10;
};
