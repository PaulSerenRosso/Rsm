// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "BurgerQuest.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UBurgerQuest : public UQuest
{
	GENERATED_BODY()

protected:
	virtual void OnQuestSuccess() override;
	virtual FString GetQuestProgress() override;

protected:
	int CurrentCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxDelivery = 5;
};
