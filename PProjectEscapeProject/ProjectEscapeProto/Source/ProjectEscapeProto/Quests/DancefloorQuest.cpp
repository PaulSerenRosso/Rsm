// Fill out your copyright notice in the Description page of Project Settings.


#include "DancefloorQuest.h"

#include "ProjectEscapeProto/Objects/LDElements/DanceFloor.h"

// void UDancefloorQuest::OnDancefloorFinished(bool WithSuccess)
// {	
// 	OnQuestFinished.Broadcast(this, WithSuccess);
// }

void UDancefloorQuest::StartQuest(UWorld* World)
{
	Super::StartQuest(World);
	for (auto NewActorSpawned : ActorSpawnedArray)
	{
		ADanceFloor* NewDanceFloor = Cast<ADanceFloor>(NewActorSpawned.Get());
		if (!NewDanceFloor) continue;
		NewDanceFloor->OnDancefloorFinished.AddDynamic(this, &UDancefloorQuest::OnQuestSuccess);
	}
}

void UDancefloorQuest::OnQuestFail()
{
	Super::OnQuestFail();
	for (auto NewActorSpawned : ActorSpawnedArray)
	{
		if (!NewActorSpawned.IsValid()) continue;
		NewActorSpawned.Get()->Destroy();
	}
	ActorSpawnedArray.Empty();
}