// Fill out your copyright notice in the Description page of Project Settings.


#include "HubGameMode.h"

#include "OnlineGameInstance.h"
#include "OnlineGameState.h"
#include "Characters/Prisoner.h"
#include "Characters/PrisonerController.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/Items/ButtonLaunchGame.h"
#include "Utilities/FunctionsUtilities.h"

void AHubGameMode::PlayerReady_Implementation(int PlayerId, float Countdown)
{
	if (PlayersIdReady.Contains(PlayerId))
	{
		PlayersIdReady.Remove(PlayerId);
		return;
	}
	PlayersIdReady.Add(PlayerId);
	TryToLaunchGame(Countdown);
}

// void AHubGameMode::PlayerNotReady_Implementation()
// {
// 	NbPlayerReady--;
// 	if (NbPlayerReady < 0) NbPlayerReady = 0;
// }

void AHubGameMode::ForceStartGame_Implementation()
{
	StartGame();
}

void AHubGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ForceRefreshButtonLaunchGame();
	
	APrisonerController* NewPlayerController = Cast<APrisonerController>(NewPlayer);
	
	PlayerControllers.Add(NewPlayerController);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, NewPlayerController]()
	{
		NewPlayerController->ProximityChatComponent->JoinProximityChannel(GameChannelCode + "_" + ChannelProximityChatSuffix);
		for (auto PC : PlayerControllers)
		{
			if (!PC) continue;
			APawn* Pawn = PC->GetPawn();
			if (!Pawn) continue;
			APrisoner *Prisoner = Cast<APrisoner>(Pawn);
			if (!Prisoner) continue;
			Prisoner->Client_My_RefreshPlayerName();
		}
	}, 1.0f, false);
	

}

int AHubGameMode::GetNbPlayersNeedToPlay()
{
	return Cast<UOnlineGameInstance>(GetWorld()->GetGameInstance())->GetNbPlayersNeedToPlay();
}

void AHubGameMode::ForceRefreshButtonLaunchGame()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (ButtonLaunchGame.IsValid())
		{
			ButtonLaunchGame->Server_RefreshNbPlayerReady();
		}
		else
		{
			ForceRefreshButtonLaunchGame();
		}
	}, 0.1f, false);
}

void AHubGameMode::StartGame()
{
	UOnlineGameInstance* ServerInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	GetWorld()->ServerTravel("MainLevel_01?listen");
}

void AHubGameMode::BeginPlay()
{
	Super::BeginPlay();

	ButtonLaunchGame = Cast<AButtonLaunchGame>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AButtonLaunchGame::StaticClass()));
	MyGameState = GetGameState<AOnlineGameState>();
	MyGameState->HostName = Cast<UOnlineGameInstance>(GetWorld()->GetGameInstance())->GetSteamAccountName();
	GameChannelCode = MyGameState->HostName + UFunctionsUtilities::GetRandomKey(12);
}

void AHubGameMode::TryToLaunchGame(float Countdown)
{
	if (GameHasBegun) return;
	if (PlayersIdReady.Num() == Cast<UOnlineGameInstance>(GetWorld()->GetGameInstance())->GetNbPlayersNeedToPlay())
	{
		GameHasBegun = true;
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			StartGame();
		}, Countdown, false);
	}
}
