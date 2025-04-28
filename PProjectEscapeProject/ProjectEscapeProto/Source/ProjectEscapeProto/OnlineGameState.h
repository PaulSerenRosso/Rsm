// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChannelId.h"
#include "OnlineGameInstance.h"
#include "GameFramework/GameState.h"
#include "Utilities/EnumsUtilities.h"
#include "Utilities/StructUtilities.h"
#include "OnlineGameState.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeaderboardChangedDelegate, const TArray<FPlayerScore>&,RefLeaderBoard );

UCLASS()
class PROJECTESCAPEPROTO_API AOnlineGameState : public AGameState
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnRep_LeaderboardChanged();
	FString GetChannelName();
	UFUNCTION(BlueprintCallable)
	void RefreshPlayerColor(int PlayerId, FLinearColor Color);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FLinearColor GetColorByPlayerId(int PlayerId) const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LeaderboardChanged)
	TArray<FPlayerScore> Leaderboard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FLeaderboardChangedDelegate OnLeaderboardChanged;
	FString HostName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Gameplay")
	TArray<ERoomType> RoomsOpened;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Gameplay")
	TArray<FColorPlayer> PlayerColors;

private:

	FString ChannelName = "";

};
