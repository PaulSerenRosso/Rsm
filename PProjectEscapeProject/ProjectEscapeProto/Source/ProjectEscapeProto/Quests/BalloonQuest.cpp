// Fill out your copyright notice in the Description page of Project Settings.


#include "BalloonQuest.h"

#include "BalloonDepot.h"
#include "ProjectEscapeProto/Objects/Items/Balloon.h"

void UBalloonQuest::BalloonFinished(bool WithSuccess)
{
	CurrentStep++;
	OnQuestUpdated.Broadcast(0, -1);
	if (CurrentStep >= MaxStep)
	{
		OnQuestFinished.Broadcast(this, true);
	}
}

void UBalloonQuest::StartQuest(UWorld* World)
{
	Super::StartQuest(World);
	CurrentStep = 0;
	for (auto NewActorSpawned : ActorSpawnedArray)
	{
		ABalloonDepot* BalloonDepot = Cast<ABalloonDepot>(NewActorSpawned);
		if (!BalloonDepot) continue;
		BalloonDepot->OnBalloonFinished.AddDynamic(this, &UBalloonQuest::BalloonFinished);
	}
}


void UBalloonQuest::OnQuestFail()
{
	Super::OnQuestFail();
	for (auto NewActorSpawned : ActorSpawnedArray)
	{
		NewActorSpawned->Destroy();
	}
	ActorSpawnedArray.Empty();
}

void UBalloonQuest::OnQuestSuccess()
{
	OnQuestUpdated.Broadcast(0, -1);
	
	if (CurrentStep >= MaxStep)
	{
		OnQuestFinished.Broadcast(this, true);
	}
}

FString UBalloonQuest::GetQuestProgress()
{
	FString QuestProgress = FString::Printf(TEXT("%d/%d"), CurrentStep, MaxStep);
	
	return QuestProgress;
}
