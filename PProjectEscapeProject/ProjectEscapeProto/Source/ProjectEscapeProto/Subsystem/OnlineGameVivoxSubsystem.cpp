// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineGameVivoxSubsystem.h"
#include "VivoxCore.h"
#include "ProjectEscapeProto/OnlineGameInstance.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"


void UOnlineGameVivoxSubsystem::InitVivox()
{
	MyVoiceModule = &FModuleManager::Get().LoadModuleChecked<FVivoxCoreModule>("VivoxCore");
	InitializeClient();
}

void UOnlineGameVivoxSubsystem::LeaveChannel(FString InputChannelName)
{
	if (!ChannelIds.Contains(InputChannelName))
	{
		return;
	}
	if (CurrentLoginSession == nullptr)
	{
		return;
	}
	ChannelId* ChannelId = GetChannelId(InputChannelName);
	if (!ChannelId->IsValid() || ChannelId->IsEmpty())
	{
		return;
	}
		IChannelSession* ChannelSession = &CurrentLoginSession->GetChannelSession(*ChannelId);
		if (ChannelSession == nullptr) return;
		ChannelSession->Disconnect();
		ChannelIds.Remove(InputChannelName);
	UE_LOG(LogTemp, Display, TEXT("Channel disconnected"));
	

}

bool UOnlineGameVivoxSubsystem::InitializeClient()
{
	if (!MyVoiceModule)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get voice module"));
		return false;
	}


	if (kDefaulEndPoint.IsEmpty() || kDefaultDomain.IsEmpty() || kDefaultIssuer.IsEmpty() || kDefaultKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Make sure server and Domain and Issuer and SecretKey is set"));
		return false;
	}
	MyVoiceClient = &MyVoiceModule->VoiceClient();
	if (!MyVoiceClient)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get voice client"));
		return false;
	}

	bool bSuccess = MyVoiceClient->Initialize() == VxErrorSuccess;
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to initialize voice client"));
		return false;
	}

	MySteamName =  UFunctionsUtilities::NormalizeString(Cast<UOnlineGameInstance>(GetWorld()->GetGameInstance())->GetSteamAccountName());
	SignIn(MySteamName, MySteamName, kDefaultExpiration);
	return true;
}

int32 UOnlineGameVivoxSubsystem::SignIn(FString InputUsername, FString InputDisplayName, FTimespan Expiration)
{
	if (InputUsername.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("make sure username is set"));
		return 99;
	}
	if (InputDisplayName.IsEmpty())
	{
		InputDisplayName = InputUsername;
	}
	AccountId TempAccountId = AccountId(*kDefaultIssuer, *InputUsername, *kDefaultDomain, InputDisplayName);

	if (!TempAccountId.IsValid())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AccountId id not valid, try again and make sure vivox cridentials and input is good"));
		return 98;
	}
	AccountIds.Add(InputUsername, TempAccountId);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Vivox: Signing in"));

	AsyncTask(ENamedThreads::GameThread, [this, TempAccountId, Expiration, InputUsername]()
	{
		ILoginSession& MyLoginSession(MyVoiceClient->GetLoginSession(TempAccountId));
		FString VivoxLoginToken = MyLoginSession.GetLoginToken(kDefaultKey, Expiration);
		ILoginSession::FOnBeginLoginCompletedDelegate OnBeginLoginCompleted;

		OnBeginLoginCompleted.BindLambda([this, &MyLoginSession, InputUsername](VivoxCoreError Error)
		{
			UE_LOG(LogTemp, Warning, TEXT("login error code: %i"), Error);
			CurrentLoginSession = &MyLoginSession;
			IsSignIn = true;
			OnVivoxInitialized.Broadcast();
		});


		MyLoginSession.BeginLogin(kDefaulEndPoint, VivoxLoginToken, OnBeginLoginCompleted);
	});

	return 0;
}


int32 UOnlineGameVivoxSubsystem::JoinChannel(FString InputChannelName,
                                             FMyChannelProperties InputChannelProperties)
{
	UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : Begin"));
	if (!this) return 99;
	if (CurrentLoginSession == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : user its not logged in"));
		return 97;
	}
	if (InputChannelName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : make sure channel name is set"));
		return 99;
	}
	

	Channel3DProperties TempChannel3DProperties(InputChannelProperties.audibleDistance,
	                                            InputChannelProperties.conversationalDistance,
	                                            InputChannelProperties.audioFadeIntensityByDistance,
	                                            InputChannelProperties.audioFadeMode);

	ChannelId TempChannelId(kDefaultIssuer, InputChannelName, kDefaultDomain, InputChannelProperties.InputChannelType,
	                        TempChannel3DProperties);
	if (!TempChannelId.IsValid())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("ALPHACODE : ChannelId id not valid, try again and make sure vivox cridentials and input is good"));
		return 98;
	}
	ChannelIds.Add(InputChannelName, TempChannelId);

	AccountId* TempAccountId = &AccountIds[MySteamName];

	ILoginSession* TempLoginSession = &MyVoiceClient->GetLoginSession(*TempAccountId);

	if (TempLoginSession->State() != LoginState::LoggedIn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : user its not logged in"));
		return false;
	}
	//IChannelSession& ChannelSession(TempLoginSession->GetChannelSession(TempChannelId));
	AsyncTask(ENamedThreads::GameThread,
	          [this, InputChannelName, TempLoginSession, TempChannelId, InputChannelProperties]()
	          {
		          IChannelSession& MyChannelSession(TempLoginSession->GetChannelSession(TempChannelId));

		          FString ConnectToken = MyChannelSession.GetConnectToken(kDefaultKey, kDefaultExpiration);
		          IChannelSession::FOnBeginConnectCompletedDelegate OnBeginConnectCompleted;
		          OnBeginConnectCompleted.BindLambda([this, &MyChannelSession, InputChannelName](VivoxCoreError CoreError)
		          {
			          UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : Join channel completed"));
		          	UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : Join channel error code: %i"), CoreError);
			          OnChannelBeginConnected.Broadcast(InputChannelName);
		          });
		          MyChannelSession.BeginConnect(InputChannelProperties.ConnectAudio, InputChannelProperties.ConnectText,
		                                        InputChannelProperties.SwitchTransmition, ConnectToken,
		                                        OnBeginConnectCompleted);

	          });
	return 0;
}

int32 UOnlineGameVivoxSubsystem::Update3dPositionalChannel(FString InputChannelName,
                                                           FVector SpeakerPosition, FVector ListenerPosition,
                                                           FVector ListenerForwardVector, FVector ListenerUpVector)
{
	if (MySteamName.IsEmpty() || InputChannelName.IsEmpty())
	{
		return 99;
	}

	if (CurrentLoginSession->State() != LoginState::LoggedIn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : user its not logged in"));
		return 97;
	}

	

	ChannelId* TempChannelId = GetChannelId(InputChannelName);
	if (TempChannelId == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : channel id not found"));
		return 96;
	}

	IChannelSession* TempChannelSession = &CurrentLoginSession->GetChannelSession(*TempChannelId);
	if (TempChannelSession == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : Channel session not found or not valid"));
		return 95;
	}

	if (TempChannelSession == nullptr)
	{
		return 96;
	}

	if (TempChannelSession->Channel().Type() != ChannelType::Positional)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : selected channel is not positional"));
		return 95;
	}
	

	return TempChannelSession->Set3DPosition(SpeakerPosition, ListenerPosition, ListenerForwardVector,
	                                         ListenerUpVector);
}


void UOnlineGameVivoxSubsystem::ModifyMicrophoneVolume(float Volume)
{
	if (MyVoiceClient)
	{
		MyVoiceClient->AudioInputDevices().SetVolumeAdjustment(Volume);
	}

	IParticipant::FOnBeginSetLocalVolumeAdjustmentCompletedDelegate BeginSetLocalVolumeCompletedCallback;
	bool IsVolumeAdjusted = false;
}

void UOnlineGameVivoxSubsystem::ModifyParticipantVolume(FString ChannelName, FString ParticipantName, float Volume)
{
	IParticipant* TempParticipant = nullptr;
	AccountId* TempAccountId = &AccountIds[MySteamName];
	ILoginSession* TempLoginSession = &MyVoiceClient->GetLoginSession(*TempAccountId);
	ChannelId* TempChannelId = GetChannelId(ChannelName);
	IChannelSession* TempChannelSession = &TempLoginSession->GetChannelSession(*TempChannelId);
	TArray<IParticipant*> Participants;
	TempChannelSession->Participants().GenerateValueArray(Participants);
	for (auto& Participant : Participants)
	{
		if (Participant->Account().Name() == ParticipantName)
		{
			TempParticipant = Participant;
			break;
		}
	}

	if (TempParticipant == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : Participant not found"));
		return;
	}
	VivoxCoreError bSuccess = TempParticipant->SetLocalVolumeAdjustment(Volume);
	if (bSuccess == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ALPHACODE : Failed to change volume for participant %s"), *TempParticipant->Account().Name());
	}
}

void UOnlineGameVivoxSubsystem::MuteUnmuteMicrophone(bool Mute)
{
	if (MyVoiceClient)
	{
		MyVoiceClient->AudioInputDevices().SetMuted(Mute);
	}
}

bool UOnlineGameVivoxSubsystem::IsLogInToChannel(FString ChannelName)
{
	if (MySteamName.IsEmpty() || ChannelName.IsEmpty())
	{
		return false;
	}

	if (CurrentLoginSession->State() != LoginState::LoggedIn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : user its not logged in"));
		return false;
	}
	
	ChannelId* TempChannelId = GetChannelId(ChannelName);
	if (TempChannelId == nullptr)
	{
		return false;
	}

	IChannelSession* TempChannelSession = &CurrentLoginSession->GetChannelSession(*TempChannelId);
	if (TempChannelSession == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : Channel session not found or not valid"));
		return false;
	}

	if(TempChannelSession->AudioState() == ConnectionState::Connected && TempChannelSession->ChannelState() == ConnectionState::Connected)
	{
		return true;
	}
	return false;
}

ChannelId* UOnlineGameVivoxSubsystem::GetChannelId(FString ChannelName)
{
	if (!ChannelIds.Contains(ChannelName))
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : channel id not found"));
		return nullptr;
	}
	return &ChannelIds[ChannelName];
}

AccountId* UOnlineGameVivoxSubsystem::GetAccountId(FString Username)
{
	if (!AccountIds.Contains(Username))
	{
		UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : account id not found"));
		return nullptr;
	}
	return &AccountIds[Username];
}
