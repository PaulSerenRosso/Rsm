// Fill out your copyright notice in the Description page of Project Settings.


#include "AnniversaryCakeQuest.h"

#include "ProjectEscapeProto/Objects/Items/AnniversaryCake.h"

void UAnniversaryCakeQuest::OnCakeArrived()
{
	OnQuestFinished.Broadcast(this, true);
}

void UAnniversaryCakeQuest::StartQuest(UWorld* World)
{
	Super::StartQuest(World);
	if (ActorSpawnedArray.Num() == 0) return;
	AAnniversaryCake* NewCake = Cast<AAnniversaryCake>(ActorSpawnedArray[0].Get());
	if (!NewCake) return;
	NewCake->OnCakeArrived.AddDynamic(this, &UAnniversaryCakeQuest::OnCakeArrived);
}

void UAnniversaryCakeQuest::OnQuestFail()
{
	Super::OnQuestFail();
	// for (auto NewActorSpawned : ActorSpawnedArray)
	// {
	// 	ABalloon* NewBalloon = Cast<ABalloon>(NewActorSpawned);
	// 	if (!NewBalloon) continue;
	// 	NewBalloon->Destroy();
	// }
	// ActorSpawnedArray.Empty();
}