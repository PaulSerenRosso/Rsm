// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HubGameMode.generated.h"

class AOnlineGameState;
class APrisonerController;
class AButtonLaunchGame;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API AHubGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void TryToLaunchGame(float Countdown);
	UFUNCTION(Server, Reliable)
	void PlayerReady(int PlayerId, float Countdown);
	// UFUNCTION(Server, Reliable)
	// void PlayerNotReady();
	UFUNCTION(Server, Reliable)
	void ForceStartGame();
	int GetNbPlayerReady() { return PlayersIdReady.Num(); }
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	int GetNbPlayersNeedToPlay();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void StartGameBP();
	void ForceRefreshButtonLaunchGame();
	
private:
	void StartGame();

protected:
	bool GameHasBegun = false;
	TWeakObjectPtr<AButtonLaunchGame> ButtonLaunchGame;
	UPROPERTY(BlueprintReadOnly, Category = "OnlineGameMode")
	TArray<APrisonerController*> PlayerControllers;
	FString GameChannelCode;
	const FString ChannelProximityChatSuffix = TEXT("PROXIMITYCHAT");
	TWeakObjectPtr<AOnlineGameState> MyGameState;
	TArray<int> PlayersIdReady;
};
