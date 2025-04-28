// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlinePlayerState.h"

#include "OnlineGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Utilities/FunctionsUtilities.h"

void AOnlinePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOnlinePlayerState, CurrentScore);
	DOREPLIFETIME(AOnlinePlayerState, PlayerStates);
	DOREPLIFETIME(AOnlinePlayerState, SteamPlayerName);
}

void AOnlinePlayerState::OnRep_CurrentScore()
{
	OnScoreUpdated.Broadcast(CurrentScore);
}

void AOnlinePlayerState::OnRep_PlayerStates()
{
	OnPlayerStatesChanged.Broadcast(OldPlayerStates, PlayerStates);
	OldPlayerStates = PlayerStates;
}


void AOnlinePlayerState::SetSteamPlayerName_Implementation(const FString& NewSteamPlayerName)
{
	SteamPlayerName = NewSteamPlayerName;
}

bool AOnlinePlayerState::SetSteamPlayerName_Validate(const FString& NewSteamPlayerName)
{
	return true;
}


