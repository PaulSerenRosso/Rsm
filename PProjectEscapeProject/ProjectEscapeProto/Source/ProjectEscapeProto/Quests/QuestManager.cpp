// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"
#include "Quest.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/DataAssets/QuestData.h"


// Sets default values
AQuestManager::AQuestManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		AOnlineGameMode* OnlineGameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
		OnlineGameMode->QuestManager = this;
	}
}

void AQuestManager::Init(UQuestManagerData* QuestManagerData)
{
	MulticastInit(QuestManagerData);
	TArray<int> AllAvailableQuests;

	if (QuestManagerData->MainQuests.Num() == 0 && QuestManagerData->SecondaryQuest.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No quests available"));
		return;
	}

	for (int i = 0; i < QuestManagerData->MainQuests.Num(); ++i)
	{
		if (AllAvailableQuests.Num() == 0 || AllAvailableQuests.Num() <= i)
			AllAvailableQuests = RefillAllQuestAvailable(QuestManagerData->MainQuests);

		MulticastInitQuest(AllAvailableQuests[i], true);
		StartQuests();
	}
	AllAvailableQuests.Empty();
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, QuestManagerData]()
	{
		TArray<int> AllAvailableQuests = RefillAllQuestAvailable(QuestManagerData->SecondaryQuest);
		for (int i = 0; i < QuestManagerData->SecondaryQuest.Num(); ++i)
		{
			if (AllAvailableQuests.Num() == 0)
				AllAvailableQuests = RefillAllQuestAvailable(QuestManagerData->SecondaryQuest);

			if (AllAvailableQuests.Num() <= i) continue;

			MulticastInitQuest(AllAvailableQuests[i], false);
			AllAvailableQuests.RemoveAt(i);
			StartQuests();
		}
	}, QuestManagerData->TimerBeforeLaunchSecondaryQuest, false);
}


void AQuestManager::MulticastInitQuest_Implementation(int Index, bool IsMainQuest)
{
	TArray<UQuestData*> Quests = IsMainQuest
		                             ? CurrentQuestManagerData->MainQuests
		                             : CurrentQuestManagerData->SecondaryQuest;

	if (Quests.Num() <= Index) return;
	UQuestData* QuestData = Quests[Index];
	if (!QuestData) return;
	UQuest* CurrentQuest = NewObject<UQuest>(GetTransientPackage(), QuestData->QuestType);
	CurrentQuest->QuestData = Quests[Index];
	CurrentQuest->IsMainQuest = IsMainQuest;
	CurrentQuests.Push(CurrentQuest);
	InitCallback();
	ShowQuestDescription();
}

TArray<int> AQuestManager::RefillAllQuestAvailable(TArray<UQuestData*> Quests)
{
	TArray<int> AllAvailableQuests;

	for (int i = 0; i < Quests.Num(); ++i)
	{
		AllAvailableQuests.Push(i);
	}
	return AllAvailableQuests;
}

void AQuestManager::MulticastInit_Implementation(UQuestManagerData* QuestManagerData)
{
	CurrentQuestManagerData = QuestManagerData;
}

void AQuestManager::EndOfQuests(bool IsSuccess)
{
	// if (IsSuccess)
	// {
	// 	OnQuestsFinished.Broadcast();
	// }
	// else
	// {
	// 	OnQuestsFinished.Broadcast();
	// 	for (auto Quest : CurrentQuests)
	// 	{
	// 		Quest->EndOfQuest();
	// 	}
	// 	CurrentQuests.Empty();
	// }
	OnQuestsFinished.Broadcast(IsSuccess);
	for (auto Quest : CurrentQuests)
	{
		Quest->EndOfQuest();
	}
	CurrentQuests.Empty();
}

float AQuestManager::GetPercentageMainQuestsFinished()
{
	float Percentage = 0;
	int MainQuestsCount = 0;

	for (auto Quest : CurrentQuests)
	{
		if (Quest->IsMainQuest)
		{
			MainQuestsCount++;
			//Percentage += Quest->GetQuestProgress();
		}
	}
	if (MainQuestsCount == 0) return 0;
	return Percentage / MainQuestsCount;
}

void AQuestManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AQuestManager, QuestsDescription);
}

void AQuestManager::Multicast_UpdateQuest_Implementation(const FText& QuestProgress)
{
	BP_OnQuestUpdated.Broadcast(QuestProgress);
}

void AQuestManager::Server_GetQuestsDescription_Implementation()
{
	QuestsDescription = FText::FromString("");
	for (auto Quest : CurrentQuests)
	{
		if (Quest->QuestData->IsSoloQuest) continue;
		QuestsDescription = FText::Format(FText::FromString("{0}\n\t- {1}"), QuestsDescription,
		                                  Quest->GetQuestDescription());
	}

	//QuestsDescription = FText::Format(FText::FromString("{0}\n\nSolo task"), QuestsDescription);

	for (auto Quest : CurrentQuests)
	{
		if (!Quest->QuestData->IsSoloQuest) continue;
		QuestsDescription = FText::Format(FText::FromString("{0}\n\t- {1}"), QuestsDescription,
		                                  Quest->GetQuestDescription());
	}
}

void AQuestManager::OnQuestFinished(UQuest* Quest, bool IsSuccess)
{
	if (!IsSuccess)
	{
		OnQuestFailed.Broadcast(Quest);
		return;
	}
	bool AllQuestsFinished = true;
	Quest->IsFinished = true;

	for (int i = 0; i < CurrentQuests.Num(); ++i)
	{
		if (!CurrentQuests[i]->IsFinished)
		{
			AllQuestsFinished = false;
		}
	}

	Server_GetQuestsDescription();

	if (AllQuestsFinished)
	{
		EndOfQuests(true);
	}
}

void AQuestManager::QuestUpdated(int ScoreAmount, int PlayerId)
{
	Server_GetQuestsDescription();
	if (PlayerId != -1)
		Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode())->AskUpdateScore(ScoreAmount, PlayerId);
	OnQuestUpdated.Broadcast(true);

	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, QuestProgress]()
	// {
	// 	Multicast_UpdateQuest(FText::AsCultureInvariant(QuestProgress));
	// }, 0.1f, false);
	Multicast_UpdateQuest(QuestsDescription);
}

void AQuestManager::StartQuests()
{
	if (!HasAuthority()) return;
	if (CurrentQuests.Num() == 0) return;

	for (auto CurrentQuest : CurrentQuests)
	{
		if (CurrentQuest->IsStarted) continue;
		CurrentQuest->OnQuestFinished.AddDynamic(this, &AQuestManager::OnQuestFinished);
		CurrentQuest->OnQuestUpdated.AddDynamic(this, &AQuestManager::QuestUpdated);
		CurrentQuest->StartQuest(GetWorld());
		OnQuestStarted.Broadcast(CurrentQuest);
		CurrentQuest->IsStarted = true;
	}
	Server_GetQuestsDescription();
	MulticastStartQuest();
	QuestUpdated(0, -1);
}

void AQuestManager::MulticastStartQuest_Implementation()
{
	HideQuestDescription();
}
