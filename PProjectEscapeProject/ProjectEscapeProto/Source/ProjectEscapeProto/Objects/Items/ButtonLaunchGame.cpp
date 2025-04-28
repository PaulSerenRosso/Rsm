// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonLaunchGame.h"

#include "Components/TextBlock.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/HubGameMode.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"

AButtonLaunchGame::AButtonLaunchGame()
{
	bReplicates = true;
	SetReplicates(true);
	
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(ItemMesh);
}

void AButtonLaunchGame::BeginPlay()
{
	Super::BeginPlay();
	// Initialisation : Texte par défaut dans le widget
	if (WidgetComponent)
	{
		UUserWidget* WidgetInstance = Cast<UUserWidget>(WidgetComponent->GetWidget());
		if (WidgetInstance)
		{
			UTextBlock* TextBlock = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("PlayerReadyText")));
			if (TextBlock)
			{
				TextBlock->SetText(FText::FromString("0 joueurs prêts"));
			}
		}
	}
}

bool AButtonLaunchGame::TryInteract_Implementation(AActor* Actor)
{
	if (IsGameReady) return false;

	APrisoner* Player = Cast<APrisoner>(Actor);
	if (!Player) return false;
	
	int PlayerId = Player->GetPlayerState()->GetPlayerId();
	
	AHubGameMode* GM = Cast<AHubGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM) return false;
	
	GM->PlayerReady(PlayerId, CountDownTimer);
	Server_RefreshNbPlayerReady();
	
	return Super::TryInteract_Implementation(Actor);
}

bool AButtonLaunchGame::TryStopInteract_Implementation(AActor* Actor)
{
	// AHubGameMode* GM = Cast<AHubGameMode>(GetWorld()->GetAuthGameMode());
	// if (!GM) return false;
	// GM->PlayerNotReady();
	// Server_RefreshNbPlayerReady();
	return Super::TryStopInteract_Implementation(Actor);
}

void AButtonLaunchGame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AButtonLaunchGame, NbPlayerReady);
	DOREPLIFETIME(AButtonLaunchGame, NbPlayersNeedToPlay);
}

void AButtonLaunchGame::Multicast_StartCountdown_Implementation(float _CountDownTimer)
{
	StartCountdown(_CountDownTimer);
}

void AButtonLaunchGame::Client_RequestRefresh()
{
	Server_RefreshNbPlayerReady();
}

void AButtonLaunchGame::Multicast_UpdateWidgetText_Implementation(const FString& Text)
{
	if (WidgetComponent)
	{
		UUserWidget* WidgetInstance = Cast<UUserWidget>(WidgetComponent->GetWidget());
		if (WidgetInstance)
		{
			UTextBlock* TextBlock = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("PlayerReadyText")));
			if (TextBlock)
			{
				TextBlock->SetText(FText::FromString(Text));
			}
		}
	}
}

void AButtonLaunchGame::Multicast_UpdateRenderText_Implementation(const FString& Text)
{
	TextRender->SetText(FText::FromString(Text));
}

void AButtonLaunchGame::Server_RefreshNbPlayerReady_Implementation()
{
	AHubGameMode* GM = Cast<AHubGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM) return;
	NbPlayerReady = GM->GetNbPlayerReady();
	NbPlayersNeedToPlay = Cast<UOnlineGameInstance>(GetWorld()->GetGameInstance())->GetNbPlayersNeedToPlay();
	if (!IsGameReady && NbPlayerReady >= NbPlayersNeedToPlay)
		IsGameReady = true;
	Multicast_UpdateRenderText(IsGameReady
		                           ? "Launching..."
		                           : FString::Printf(TEXT("%d/%d"), NbPlayerReady, NbPlayersNeedToPlay));
	if (IsGameReady)
	{
		Multicast_StartCountdown(CountDownTimer);
	}
}
