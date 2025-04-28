// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineGameState.h"

#include "Net/UnrealNetwork.h"
#include "Utilities/FunctionsUtilities.h"

void AOnlineGameState::OnRep_LeaderboardChanged()
{
	OnLeaderboardChanged.Broadcast(Leaderboard);
}

FString AOnlineGameState::GetChannelName()
{
	if (ChannelName.IsEmpty())
	{
		ChannelName = HostName + "Channel" + UFunctionsUtilities::GetRandomKey(12);
	}
	return ChannelName;
}

void AOnlineGameState::RefreshPlayerColor(int PlayerId, FLinearColor Color)
{
	for (auto PlayerCol : PlayerColors)
	{
		if (PlayerCol.PlayerId == PlayerId)
		{
			PlayerCol.Color = Color;
			return;
		}
	}
	PlayerColors.Add(FColorPlayer{PlayerId, Color});
}

FLinearColor AOnlineGameState::GetColorByPlayerId(int PlayerId) const
{
	for (auto PlayerCol : PlayerColors)
	{
		if (PlayerCol.PlayerId == PlayerId)
		{
			return PlayerCol.Color;
		}
	}
	return FLinearColor::White;
}

void AOnlineGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOnlineGameState, Leaderboard);
	DOREPLIFETIME(AOnlineGameState, RoomsOpened);
	DOREPLIFETIME(AOnlineGameState, PlayerColors);
}
