// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineGameState.h"
#include "OnlinePlayerState.h"
#include "GameFramework/GameMode.h"
#include "IA/AICharacter.h"
#include "OnlineGameMode.generated.h"
class AEntitiesManager;
class UGameplayPhasis;
class UQuest;
class UMascotAvatar;
class UMascotData;
class AMascot;
class APrisonerController;
enum class EPlayerStates : uint8;
class AElevatorManager;
class AQuestManager;
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartGameDelegate);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndGameDelegate);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestsFinishedDelegate, bool, IsSuccess);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerFinishedDelegate);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomUpdateDelegate, ERoomType, RoomUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FtimerOnlineUpdate, float, Time);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPhasisChangedDelegate, int, CurrentPhasisIndex);

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API AOnlineGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void AddPC(APrisonerController* Controller);
	void RemovePC(APrisonerController* controller);
	TWeakObjectPtr<APrisonerController> GetNextOrPrevPC(TWeakObjectPtr<APrisonerController> Current, bool IsNext);
	TWeakObjectPtr<APrisonerController> GetNextOrPrevAlivePC(TWeakObjectPtr<APrisonerController> Current, bool IsNext);
	TArray<APrisonerController*> GetAllPCs() { return PlayerControllers; }
	UFUNCTION(BlueprintCallable, Category = "OnlineGameMode")
	void AskUpdateScore(int Score, int PlayerId, bool Forced = false);
	void AskUpdatePlayerState(EPlayerStates PlayerState, int PlayerId, bool PlayerStateValue);
	int GetScore(int PlayerId);
	bool CheckIsAllDeadOrSpectator();
	FString GetHostPlayerName() const;
	UFUNCTION()
	void GameOver();
	virtual void Tick(float DeltaSeconds) override;
	void SetMainTimer(float Timer);
	void PrisonerTakeElevator(int Index);
	float GetCurrentMainTimer() const { return CurrentMainTimer; }
	TArray<ERoomType> GetRoomsOpened() const { return MyGameState->RoomsOpened; }
	void OpenRoom(ERoomType RoomType);
	void CloseRoom(ERoomType RoomType);
	int GetPlayerCount() const;
	int GetPhasisCount() const;
	int GetCurrentPhasisIndex() const;
	void AskStartGame();
	void AddTime(float Time);
	void StartNewPhasis();
	UFUNCTION(BlueprintCallable)
	void RefreshPlayerColor(int PlayerId, FLinearColor Color);
	void AskDestroyEntity(AAICharacter* Character);

protected:
	void CheckLeaderboard();
	static bool ComparePlayerScore(const FPlayerScore& First, const FPlayerScore& Second);
	UFUNCTION()
	void QuestsFinished(bool IsSuccess);
	virtual void Logout(AController* Exiting) override;

private:


	void EndExperience(bool WithSuccess);
	void EndGame(bool WithSuccess);

public:
	FStartGameDelegate OnGameReadyToStart;
	UPROPERTY(BlueprintAssignable, Category = "OnlineGameMode")
	FStartGameDelegate OnStartGame;
	FEndGameDelegate OnEndGame;
	TWeakObjectPtr<AQuestManager> QuestManager;
	TWeakObjectPtr<AElevatorManager> ElevatorManager;
	FOnQuestsFinishedDelegate OnQuestsFinished;
	UPROPERTY(BlueprintAssignable, Category = "OnlineGameMode")
	FtimerOnlineUpdate OnTimerUpdate;
	FOnTimerFinishedDelegate OnTimerFinished;
	FOnRoomUpdateDelegate OnRoomOpened;
	FOnRoomUpdateDelegate OnRoomClosed;

	UPROPERTY(BlueprintAssignable, Category = "OnlineGameMode")
	FPhasisChangedDelegate NewPhasisBegan;
	UPROPERTY(BlueprintAssignable, Category = "OnlineGameMode")
	FPhasisChangedDelegate PreviousPhasisEnded;
	UPROPERTY(BlueprintAssignable, Category = "OnlineGameMode")
	FOnTimerFinishedDelegate IsLastPhasis;

protected:
	int PhasisCount;
	int CurrentPhasisIndex = -1;
	UPROPERTY(EditAnywhere, Category = "Mascot")
	TArray<UMascotData*> MascotDatas;
	UPROPERTY(EditAnywhere, Category = "Mascot")
	TSubclassOf<UMascotAvatar> MascotAvatarClass;
	UPROPERTY(BlueprintReadOnly)
	TArray<APrisonerController*> PlayerControllers;
	FString GameChannelCode;
	const FString ChannelProximityChatSuffix = TEXT("PROXIMITYCHAT");

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TArray<UGameplayPhasis*> Phasis;
	UPROPERTY()
	UGameplayPhasis* CurrentPhasis = nullptr;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float DelayBeforeStartTimer = 1.0f;

private:
	bool GameHasBegun = false;
	TArray<AOnlinePlayerState*> PlayerStates;
	TWeakObjectPtr<AOnlineGameState> MyGameState;
	TWeakObjectPtr<AEntitiesManager> EntitiesManager;
	TMap<APlayerController*, AMascot*> Mascots;
	FTimerHandle RetriggerableTimerHandle;
	float CurrentMainTimer = 0.0f;
};
