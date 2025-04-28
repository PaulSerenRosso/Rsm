// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionTimerManager.h"

TArray<FActionTimer*> AActionTimerManager::ActionTimers;

// Sets default values
AActionTimerManager::AActionTimerManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AActionTimerManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AActionTimerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < ActionTimers.Num(); i++)
	{
		ActionTimers[i]->Duration -= DeltaTime;
		if (ActionTimers[i]->Duration <= 0)
		{
			if (ActionTimers[i]->OnActionTimerManagerTick.IsBound())
			{
				ActionTimers[i]->OnActionTimerManagerTick.Broadcast(ActionTimers[i]->Id);
				if (ActionTimers[i]->Loop)
				{
					ActionTimers[i]->Duration = ActionTimers[i]->MaxDuration;
				}
				else
				{
					ActionTimers.RemoveAt(i);
					i--;
				}
			}
			else
			{
				ActionTimers.RemoveAt(i);
				i--;
			}
		}
	}
}

FActionTimer* AActionTimerManager::AddActionTimer(float Duration, int _Id, bool IsLooping)
{
	FActionTimer* NewTimer = new FActionTimer();
	NewTimer->MaxDuration = Duration;
	NewTimer->Duration = Duration;
	NewTimer->Id = _Id;
	NewTimer->Loop = IsLooping;
	ActionTimers.Add(NewTimer);
	return NewTimer;
}

void AActionTimerManager::RemoveActionTimer(FOnActionTimerManagerTick Timer)
{
	for (int i = 0; i < ActionTimers.Num(); i++)
	{
		if (ActionTimers[i]->MaxDuration)
		{
			ActionTimers.RemoveAt(i);
			break;
		}
	}
}
