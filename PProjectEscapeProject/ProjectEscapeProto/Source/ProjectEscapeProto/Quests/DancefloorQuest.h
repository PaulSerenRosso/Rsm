// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "DancefloorQuest.generated.h"


UCLASS()
class PROJECTESCAPEPROTO_API UDancefloorQuest : public UQuest
{
	GENERATED_BODY()

protected:
	virtual void StartQuest(UWorld* World) override;
	virtual void OnQuestFail() override;
};
