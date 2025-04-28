// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionTimerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionTimerManagerTick, int, Id);

USTRUCT()
struct FActionTimer
{
	GENERATED_BODY()

	UPROPERTY()
	float MaxDuration = 0;
	UPROPERTY()
	float Duration = 0;
	UPROPERTY()
	int Id = -1;
	UPROPERTY()
	bool Loop = false;

	UPROPERTY()
	FOnActionTimerManagerTick OnActionTimerManagerTick;
};

UCLASS()
class PROJECTESCAPEPROTO_API AActionTimerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActionTimerManager();
	virtual void Tick(float DeltaTime) override;
	static FActionTimer* AddActionTimer(float Duration, int _Id, bool IsLooping = false);
	static void RemoveActionTimer(FOnActionTimerManagerTick Timer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	static TArray<FActionTimer *> ActionTimers;

};
