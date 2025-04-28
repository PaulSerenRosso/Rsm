// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorQuest.h"

#include "ProjectEscapeProto/OnlineGameInstance.h"
#include "ProjectEscapeProto/DataAssets/GeneratorQuestData.h"
#include "ProjectEscapeProto/Objects/LDElements/Generator.h"

void UGeneratorQuest::StartQuest(UWorld* World)
{
	Super::StartQuest(World);
	GeneratorQuestData = Cast<UGeneratorQuestData>(QuestData);
	for (auto NewActorSpawned : ActorSpawnedArray)
	{
		AGenerator* Generator = Cast<AGenerator>( NewActorSpawned.Get());
		if (!Generator) return;
		Generator->OnRepared.AddDynamic(this, &UGeneratorQuest::OnGeneratorRepared);
		Generator->OnTryToRepared.AddDynamic(this, &UGeneratorQuest::OnGeneratorTryToRepared);
		++UnachievedGenerators;
	}
}

void UGeneratorQuest::OnQuestFail()
{
	Super::OnQuestFail();
	for (auto NewActorSpawned : ActorSpawnedArray)
	{
		AGenerator* Generator = Cast<AGenerator>(NewActorSpawned.Get());
		if (!Generator) return;
		Generator->OnRepared.RemoveDynamic(this, &UGeneratorQuest::OnGeneratorRepared);
		Generator->OnTryToRepared.RemoveDynamic(this, &UGeneratorQuest::OnGeneratorTryToRepared);
	}
}

void UGeneratorQuest::OnGeneratorRepared(AActor* HitBy)
{
	if (--UnachievedGenerators > 0) return;
	OnQuestFinished.Broadcast(this, true);
}

void UGeneratorQuest::OnGeneratorTryToRepared(AActor* HitBy)
{
	APawn* Pawn = Cast<APawn>(HitBy);
	if (!Pawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HitBy is not a pawn")));
		return;
	}
	APlayerState* PlayerState = Pawn->GetPlayerState();
	if (!PlayerState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("PlayerState is null")));
		return;
	}
	int PlayerId = Cast<APawn>(HitBy)->GetPlayerState()->GetPlayerId();
	OnQuestUpdated.Broadcast(GeneratorQuestData->ScoreAmountByHit, PlayerId);
}
