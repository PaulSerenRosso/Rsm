// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "Subsystem/BinaryDataSubsystem.h"
#include "MapInfoSubsystem.h"
#include "Subsystem/OnlineGameVivoxSubsystem.h"
#include "Engine/GameInstance.h"
#include "Utilities/StructUtilities.h"
#include "OnlineGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UOnlineGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "OnlineGameMode")
	static FString GetSteamAccountName();
	UFUNCTION(BlueprintCallable, Category = "OnlineGameMode")
	static void SetSteamAccountName(FText SteamName);

	int GetNbPlayersNeedToPlay() const { return NbPlayersNeedToPlay; }

private:
	virtual void Init() override;

public:
	UMapInfoSubsystem* MapInfoSubsystem;
	UOnlineGameVivoxSubsystem* OnlineGameVivoxSubsystem;
	bool NeedToInitVivox = true;

	UBinaryDataSubsystem* BinaryDataSubsystem;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Online")
	int NbPlayersNeedToPlay = 2;
	UPROPERTY(BlueprintReadWrite, Category = "Online")
	FString RoomCode = "";

	static FString SteamUsername;
	UPROPERTY(BlueprintReadWrite, Category = "Online")
	FLinearColor PlayerColor = FLinearColor::White;
	
};
