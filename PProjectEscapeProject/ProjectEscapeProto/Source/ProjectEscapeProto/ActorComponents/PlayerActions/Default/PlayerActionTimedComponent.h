// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerActionStateComponent.h"

#include "PlayerActionTimedComponent.generated.h"


class APrisoner;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UPlayerActionTimedComponent : public UPlayerActionStateComponent

{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerActionTimedComponent();
	void MyServer_ExecuteAction(const TArray<uint8>& Payload =  TArray<uint8>(), const TArray<AActor*>& Actors = TArray<AActor*>()) override;
	void MyServer_StopAction(const TArray<uint8>& Payload = TArray<uint8>(), const TArray<AActor*>& Actors = TArray<AActor*>()) override;
	virtual void EndTimerAction();
	virtual float GetCurrentTime();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FTimerHandle ActionTimerHandle;
	
	
	

protected:
	
	
};
