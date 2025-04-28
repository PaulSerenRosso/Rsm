// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VivoxCore.h"

#include "OnlineGameVivoxSubsystem.generated.h"

struct FMyChannelProperties;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVivoxInitializedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVivoxChannelBeginConnectedDelegate, FString, ChannelName);

UCLASS()
class PROJECTESCAPEPROTO_API UOnlineGameVivoxSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void InitVivox();
	void LeaveChannel(FString InputChannelName);
	int32 JoinChannel(FString InputChannelName,
	                  FMyChannelProperties InputChannel3dProperties);
	ChannelId* GetChannelId(FString ChannelName);
	AccountId* GetAccountId(FString Username);
	int32 Update3dPositionalChannel(FString InputChannelName, FVector SpeakerPosition,
	                                FVector ListenerPosition, FVector ListenerForwardVector, FVector ListenerUpVector);
	UFUNCTION(BlueprintCallable)
	void ModifyMicrophoneVolume(float Volume);
	UFUNCTION(BlueprintCallable)
	void ModifyParticipantVolume(FString ChannelName, FString ParticipantName, float Volume);
	UFUNCTION(BlueprintCallable)
	void MuteUnmuteMicrophone(bool Mute);
	bool IsLogInToChannel(FString ChannelName);

protected:
	//Client
	bool InitializeClient();
	int32 SignIn(FString InputUsername, FString InputDisplayName, FTimespan Expiration);

public:
	FVivoxInitializedDelegate OnVivoxInitialized;
	FVivoxChannelBeginConnectedDelegate OnChannelBeginConnected;
	IClient* MyVoiceClient = nullptr;
	FVivoxCoreModule* MyVoiceModule;
	ILoginSession* CurrentLoginSession;
	bool IsSignIn = false;

	

protected:
	FString MySteamName = "";
	TMap<FString, AccountId> AccountIds;
	TMap<FString, ChannelId> ChannelIds;

	const FString kDefaulEndPoint = "https://unity.vivox.com/appconfig/90721-rsm-16045-udash";
	const FString kDefaultDomain = "mtu1xp.vivox.com";
	const FString kDefaultIssuer = "90721-rsm-16045-udash";
	const FString kDefaultKey = "eR6W7OEEd5OanoQgNVSqB1FGYoTCOaX1";
	const FTimespan kDefaultExpiration = FTimespan::FromDays(10);
};
