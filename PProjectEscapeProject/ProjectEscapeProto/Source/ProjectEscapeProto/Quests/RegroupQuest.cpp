// Fill out your copyright notice in the Description page of Project Settings.


#include "RegroupQuest.h"

#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/RegroupQuestData.h"
#include "ProjectEscapeProto/Objects/LDElements/PlayerGroupPoint.h"

void URegroupQuest::StartQuest(UWorld* World)
{
	Super::StartQuest(World);
	RegroupQuestData = Cast<URegroupQuestData>(QuestData);
	GameMode = Cast<AOnlineGameMode>(World->GetAuthGameMode());
	for (auto NewActorSpawned : ActorSpawnedArray)
	{
		PlayerGroupPoint = Cast<APlayerGroupPoint>(NewActorSpawned);
		if (PlayerGroupPoint)
		{
			PlayerGroupPoint->OnPlayerJoin.AddDynamic(this, &URegroupQuest::OnPlayerJoinGroup);
			PlayerGroupPoint->OnPlayerLeave.AddDynamic(this, &URegroupQuest::OnPlayerLeaveGroup);
			break;
		}
	}
}

void URegroupQuest::OnPlayerLeaveGroup(APrisoner* Prisoner)
{
	OnQuestUpdated.Broadcast(0, -1);
	
}

void URegroupQuest::OnPlayerJoinGroup(APrisoner* Prisoner)
{
	

	if(GameMode->GetPlayerCount() == PlayerGroupPoint->PrisonersGrouped.Num())
	{
		for (auto PrisonerGrouped : PlayerGroupPoint->PrisonersGrouped)
		{
			APlayerState* PlayerState = PrisonerGrouped->GetPlayerState();
			if (!PlayerState)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("PlayerState is null")));
				return;
			}
			if(RegroupQuestData == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data is null")));
			}
			int PlayerId = PlayerState->GetPlayerId();
			OnQuestUpdated.Broadcast(RegroupQuestData->ScoreWhenQuestSucceeded, PlayerId);
		}
		OnQuestFinished.Broadcast(this, true);
	}
}


