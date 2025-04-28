// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "AnniversaryCakeQuest.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UAnniversaryCakeQuest : public UQuest
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnCakeArrived();
	virtual void StartQuest(UWorld* World) override;
	virtual void OnQuestFail() override;
};
