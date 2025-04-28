// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Utilities/EnumsUtilities.h"
#include "OnlinePlayerState.generated.h"

/**
 * 
 */
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreUpdatedDelegate, int, Score);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerStateChangedDelegate, int32, OldPlayerStates, int32,
                                             NewPlayerStates);

UCLASS()
class PROJECTESCAPEPROTO_API AOnlinePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnRep_CurrentScore();
	UFUNCTION()
	virtual void OnRep_PlayerStates();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetSteamPlayerName(const FString& NewSteamPlayerName);

public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentScore)
	int CurrentScore = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FScoreUpdatedDelegate OnScoreUpdated;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerStates, meta=(Bitmask))
	int32 PlayerStates = 0;
	UPROPERTY(BlueprintReadWrite)
	FLinearColor PlayerColor = FLinearColor::White;
	int32 OldPlayerStates;
	FPlayerStateChangedDelegate OnPlayerStatesChanged;
	UPROPERTY(Replicated)
	FString SteamPlayerName = "Unknown";
	UPROPERTY(BlueprintReadOnly, Replicated)
	bool SucceedToEscape = false;
	bool IsEndGame = false;
};
