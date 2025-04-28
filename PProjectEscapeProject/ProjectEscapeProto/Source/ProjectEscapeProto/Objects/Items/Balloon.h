// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Balloon.generated.h"

class UBalloonData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBalloonFinishedEvent, bool, WithSuccess);

UCLASS()
class PROJECTESCAPEPROTO_API ABalloon : public AItem
{
	GENERATED_BODY()

public:
	ABalloon();
	void ValidateBalloon();
	UPROPERTY(BlueprintAssignable, Category = "Balloon")
	FBalloonFinishedEvent OnBalloonFinished;
	virtual void Tick(float DeltaSeconds) override;
	
};
