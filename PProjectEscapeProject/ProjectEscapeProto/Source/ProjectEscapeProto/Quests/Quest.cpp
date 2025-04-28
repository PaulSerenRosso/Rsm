// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"

#include "TaskBase.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/OnlineGameInstance.h"
#include "ProjectEscapeProto/DataAssets/QuestData.h"
#include "ProjectEscapeProto/Managers/MapInfoManager.h"
#include "ProjectEscapeProto/Objects/Items/Item.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"
#include "ProjectEscapeProto/Utilities/StructUtilities.h"
#include "Widgets/Text/SRichTextBlock.h"

void UQuest::StartQuest(UWorld* World)
{
	WorldRef = World;
	TArray<FActorSpawnInfo> QuestActors = QuestData->QuestActors;
	for (FActorSpawnInfo ActorSpawnInfo : QuestActors)
	{
		TArray<AActor*> Actors = Cast<UOnlineGameInstance>(World->GetGameInstance())->MapInfoSubsystem->MapInfoManager->
			GetActorWithTag(ActorSpawnInfo.TagOfSpawnedActors);
		TArray<int> Indexes;

		for (int i = 0; i < Actors.Num(); i++) Indexes.Add(i);

		Indexes = UFunctionsUtilities::ShuffleArray<int>(Indexes);

		for (int i = 0; i < ActorSpawnInfo.NbToSpawn; i++)
		{
			if (Indexes.Num() <= i) break;
			if (!Actors[Indexes[i]]) continue;
			FTransform MyTransform = Actors[Indexes[i]]->GetActorTransform();

			if (!World)
			{
				UE_LOG(LogTemp, Error, TEXT("Could not retrieve a valid world context!"));
				continue;
			}

			if (!ActorSpawnInfo.ActorType)
			{
				UE_LOG(LogTemp, Error, TEXT("ActorType is nullptr on the client!"));
				continue;
			}

			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (ASpawnActor* ActorSpawned = Cast<ASpawnActor>(
				World->SpawnActor(ActorSpawnInfo.ActorType, &MyTransform, SpawnInfo)))
			{
				TWeakObjectPtr<ASpawnActor> WeakPtrSpawn = ActorSpawned;
				ActorSpawnedArray.Add(WeakPtrSpawn);
				ATaskBase * Task = Cast<ATaskBase>(ActorSpawned);
				if (Task)
				{
					ActorSpawned->SetUsable(false);
					Task->OnTaskCompleted.AddDynamic(this, &UQuest::OnQuestSuccess);
				}
			}
		}
	}
}

void UQuest::OnQuestFail()
{
}

void UQuest::OnQuestSuccess()
{
	OnQuestUpdated.Broadcast(0, -1);
	
	for (auto CurrentQuest : ActorSpawnedArray)
	{
		if (!CurrentQuest.IsValid()) continue;
		if (CurrentQuest->IsCountHasAnObjective && CurrentQuest.Get()->GetInteractable()) return;
	}
		OnQuestFinished.Broadcast(this, true);
}

void UQuest::EndOfQuest()
{
	for (auto ActorSpawn : ActorSpawnedArray)
	{
		if (!ActorSpawn.IsValid()) continue;
		ActorSpawn.Get()->DeactivateInteraction();
	}
	ActorSpawnedArray.Empty();
}

FText UQuest::GetQuestDescription()
{
	FText QuestDescription = FText::FromString("");
	if (IsFinished) QuestDescription = FText::FromString("<Strike>");
	MaxCount = 0;
	CurrentCount = 0;

	QuestDescription = FText::Format(FText::FromString("{0}{1}"), QuestDescription,
	                                 FText::FromString(QuestData->QuestDescription));
	if (QuestData->IsAQuantifiedQuest)
	{
		for (auto ActorSpawn : ActorSpawnedArray)
		{
			if (!ActorSpawn.IsValid()) continue;

			if (ActorSpawn.Get()->IsCountHasAnObjective)
			{
				MaxCount++;
				if (!ActorSpawn.Get()->GetInteractable())
					CurrentCount++;
			}

			ActorSpawn.Get()->ActivateWallhack();
		}
		QuestDescription = FText::Format(FText::FromString("{0} ({1})"), QuestDescription, FText::AsCultureInvariant(GetQuestProgress()));
	}

	if (IsFinished) QuestDescription = FText::Format(FText::FromString("{0}</>"), QuestDescription);
	return QuestDescription;
}

FString UQuest::GetQuestProgress()
{
	float Progress = 0;
	float MaxCountProgress = 0;
	float CurrentCountProgress = 0;
	for (auto ActorSpawn : ActorSpawnedArray)
	{
		if (!ActorSpawn.IsValid()) continue;
		if (ActorSpawn.Get()->IsCountHasAnObjective)
		{
			MaxCountProgress++;
			if (!ActorSpawn.Get()->GetInteractable())
				CurrentCountProgress++;
		}
	}
	if (MaxCount > 0)
		Progress = CurrentCountProgress / MaxCount;
	return FString::SanitizeFloat(Progress * 100) + "%";
}
