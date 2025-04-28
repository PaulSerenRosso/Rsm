// Fill out your copyright notice in the Description page of Project Settings.


#include "ProximityChat.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "ProjectEscapeProto/OnlineGameInstance.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/Characters/PrisonerController.h"
#include "ProjectEscapeProto/DataAssets/ProximityChatData.h"
#include "ProjectEscapeProto/Utilities/StructUtilities.h"


void UProximityChat::Init(bool IsLocal)
{
	IsLocalProximityChat = IsLocal;
	SetIsReplicated(true);
	if (IsLocalProximityChat)
	{
		OnlineGameInstance = Cast<UOnlineGameInstance>(GetOwner()->GetGameInstance());
		OnlineGameVivoxSubsystem = OnlineGameInstance->GetSubsystem<UOnlineGameVivoxSubsystem>();
		if (OnlineGameVivoxSubsystem->IsSignIn)
		{
			OnlineGameVivoxSubsystem->OnChannelBeginConnected.
			                          AddDynamic(this, &UProximityChat::OnBeginConnectedChannel);
		}
	}
}

// Sets default values for this component's properties
UProximityChat::UProximityChat(): OnlineGameVivoxSubsystem(nullptr), ProximityChatData(nullptr), CurrentSession(),
                                  OnlineGameInstance(nullptr),
                                  SoundEmitter(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UProximityChat::JoinProximityChannel_Implementation(const FString& KeyCode)
{
	FMyChannelProperties ChannelProperties;
	ChannelProperties.audibleDistance = ProximityChatData->AudibleDistance;
	ChannelProperties.conversationalDistance = ProximityChatData->ConversationalDistance;
	ChannelProperties.audioFadeIntensityByDistance = ProximityChatData->AudioFadeIntensityByDistance;
	ChannelProperties.audioFadeMode = ProximityChatData->AudioFadeModel;
	ChannelProperties.ConnectAudio = true;
	ChannelProperties.ConnectText = false;
	ChannelProperties.SwitchTransmition = true;
	ProximityChannelName = KeyCode;
	OnlineGameVivoxSubsystem->JoinChannel(KeyCode, ChannelProperties);
}


// Called every frame
void UProximityChat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickPosition(Cast<APlayerController>(GetOwner())->GetPawn());
	if (!OnlineGameVivoxSubsystem.Get() || !OnlineGameInstance.Get()) return;
	if (!Mine)
	{
		AccountId* TempAccountId = OnlineGameVivoxSubsystem->GetAccountId(OnlineGameInstance->GetSteamAccountName());
		if (!TempAccountId)
		{
			UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : account id not found in TickComponent"));
			//XXXX
			
			return;
		}
		ILoginSession* TempLoginSession = &OnlineGameVivoxSubsystem->MyVoiceClient->GetLoginSession(*TempAccountId);
		if (!TempLoginSession)
		{
			UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : login session not found in TickComponent"));
			return;
		}
		ChannelId* TempChannelId = OnlineGameVivoxSubsystem->GetChannelId(ProximityChannelName);
		if (!TempChannelId)
		{
			UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : channel id not found in TickComponent"));
			return;
		}
		IChannelSession* TempChannelSession = &TempLoginSession->GetChannelSession(*TempChannelId);
		if (!TempChannelSession)
		{
			UE_LOG(LogTemp, Warning, TEXT("ALPHACODE : channel session not found in TickComponent"));
			return;
		}
		
		TArray<IParticipant*> Participants;
		TempChannelSession->Participants().GenerateValueArray(Participants);
		for (auto& Participant : Participants)
		{
			if (Participant->Account().Name() == OnlineGameInstance->GetSteamAccountName())
			{
				Mine = Participant;
				break;
			}
		}
		TempLoginSession->SetParticipantSpeakingUpdateRate(ParticipantSpeakingUpdateRate::Update10Hz);
		return;
	}
	if (!Mine->SpeechDetected()) return;
	if (!SoundEmitter.IsValid())
		SoundEmitter = Cast<UPawnNoiseEmitterComponent>(
			GetOwner()->GetComponentByClass(UPawnNoiseEmitterComponent::StaticClass()));
	float RatioNoise = Mine->AudioEnergy() * ProximityChatData->RatioNoiseEmittedByVoiceActivity;
	APrisonerController *PC = Cast<APrisonerController>(GetOwner());
	if (!PC) return;
		
	APrisoner *Prisoner = Cast<APrisoner>(PC->GetPawn());
	if (!Prisoner) return;
	Prisoner->GenerateSound(RatioNoise);
}


void UProximityChat::OnParticipantAdded(const IParticipant& NewParticipant)
{
	FString ParticipantName = NewParticipant.Account().Name();

	RefreshParticipantsVolumePrint();
}

void UProximityChat::OnParticipantRemoved(const IParticipant& RemovedParticipant)
{
	FString ParticipantName = RemovedParticipant.Account().Name();

	RefreshParticipantsVolumePrint(ParticipantName);
}


void UProximityChat::RefreshParticipantsVolumePrint(FString ParticipantNameToAvoid)
{
	TArray<FString> ParticipantsName;
	TArray<float> ParticipantsVolume;

	AccountId* TempAccountId = OnlineGameVivoxSubsystem->GetAccountId(OnlineGameInstance->GetSteamAccountName());
	if (!TempAccountId) return;
	if (!OnlineGameVivoxSubsystem->MyVoiceClient) return;
	ILoginSession &TempLoginSession = OnlineGameVivoxSubsystem->MyVoiceClient->GetLoginSession(*TempAccountId);
	ChannelId* TempChannelId = OnlineGameVivoxSubsystem->GetChannelId(ProximityChannelName);
	IChannelSession* TempChannelSession = &TempLoginSession.GetChannelSession(*TempChannelId);
	TArray<IParticipant*> Participants;
	TempChannelSession->Participants().GenerateValueArray(Participants);

	for (auto& Participant : Participants)
	{
		FString ParticipantName = Participant->Account().Name();
		if (OnlineGameInstance->GetSteamAccountName() == ParticipantName || ParticipantNameToAvoid == ParticipantName)
			continue;
		ParticipantsName.Add(ParticipantName);
		ParticipantsVolume.Add(Participant->LocalVolumeAdjustment());
	}
	OnParticipantVolumeUpdated.Broadcast(ParticipantsName, ParticipantsVolume);
}

void UProximityChat::ModifyParticipantsVolume(FString ParticipantName, float Volume)
{
	if (OnlineGameVivoxSubsystem->IsLogInToChannel(ProximityChannelName))
	{
		OnlineGameVivoxSubsystem->ModifyParticipantVolume(ProximityChannelName, ParticipantName, Volume);
	}
}

void UProximityChat::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UProximityChat::OnBeginConnectedChannel(FString ChannelName)
{
	if (ProximityChannelName == ChannelName)
	{
		CurrentSession =
			&OnlineGameVivoxSubsystem->CurrentLoginSession->GetChannelSession(
				*OnlineGameVivoxSubsystem->GetChannelId(ChannelName));
		CurrentSession->EventAfterParticipantAdded.AddUObject(this, &UProximityChat::OnParticipantAdded);
		CurrentSession->EventBeforeParticipantRemoved.AddUObject(this, &UProximityChat::OnParticipantRemoved);
	}
}

void UProximityChat::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (IsLocalProximityChat)
	{
		if (IsConnectedToProximityChat())
		{
			OnlineGameVivoxSubsystem->LeaveChannel(ProximityChannelName);
		}
	}
}

bool UProximityChat::IsConnectedToProximityChat()
{
	if (OnlineGameVivoxSubsystem.IsValid() && !ProximityChannelName.IsEmpty())
	{
		if (OnlineGameVivoxSubsystem->IsLogInToChannel(ProximityChannelName))
		{
			return true;
		}
	}
	return false;
}


void UProximityChat::TickPosition(APawn* PlayerPawn)
{
	if (!IsLocalProximityChat) return;

	if (IsConnectedToProximityChat())
	{
		OnlineGameVivoxSubsystem->Update3dPositionalChannel(ProximityChannelName, PlayerPawn->GetActorLocation(),
		                                                    PlayerPawn->GetActorLocation(),
		                                                    PlayerPawn->GetActorForwardVector(),
		                                                    PlayerPawn->GetActorUpVector());
	}
}
