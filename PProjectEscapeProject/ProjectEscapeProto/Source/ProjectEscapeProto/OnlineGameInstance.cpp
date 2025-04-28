// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineGameInstance.h"

#include "Objects/Items/Item.h"
#include "Utilities/FunctionsUtilities.h"
#include "Utilities/StructUtilities.h"

FString UOnlineGameInstance::SteamUsername = "";

void UOnlineGameInstance::Init()
{
	Super::Init();
	MapInfoSubsystem = GetSubsystem<UMapInfoSubsystem>();
	OnlineGameVivoxSubsystem = GetSubsystem<UOnlineGameVivoxSubsystem>();
	BinaryDataSubsystem = GetSubsystem<UBinaryDataSubsystem>();
}

FString UOnlineGameInstance::GetSteamAccountName()
{
	if (SteamUsername.IsEmpty())
	{
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
		if (OnlineSub)
		{
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
			if (Identity.IsValid())
			{
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);

				if (UserId.IsValid())
				{
					SteamUsername = Identity->GetPlayerNickname(*UserId);
				}
			}
		}
		else
			SteamUsername = "Unknown" + UFunctionsUtilities::GetRandomKey(6);
	}
	
	return SteamUsername;
}

void UOnlineGameInstance::SetSteamAccountName(FText SteamName)
{
	SteamUsername = SteamName.ToString();
}
