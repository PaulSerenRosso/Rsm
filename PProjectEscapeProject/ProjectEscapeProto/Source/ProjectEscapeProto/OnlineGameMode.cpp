// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineGameMode.h"

#include "GameplayPhasis.h"
#include "OnlineGameInstance.h"
#include "OnlineGameState.h"
#include "OnlinePlayerState.h"
#include "ActorComponents/InventorySystem.h"
#include "Characters/Prisoner.h"
#include "Characters/PrisonerController.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/ElevatorManager.h"
#include "Managers/EntitiesManager.h"
#include "Mascot/Mascot.h"
#include "Mascot/MascotAvatar.h"
#include "Objects/Items/Item.h"
#include "Quests/QuestManager.h"
#include "Utilities/EnumsUtilities.h"
#include "Utilities/FunctionsUtilities.h"

typedef char* Char;

void AOnlineGameMode::BeginPlay()
{
	Super::BeginPlay();
	MyGameState = GetGameState<AOnlineGameState>();
	MyGameState->HostName = Cast<UOnlineGameInstance>(GetWorld()->GetGameInstance())->GetSteamAccountName();
	GameChannelCode = MyGameState->HostName + UFunctionsUtilities::GetRandomKey(12);
	PhasisCount = Phasis.Num();
	EntitiesManager = Cast<AEntitiesManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AEntitiesManager::StaticClass()));
}

void AOnlineGameMode::AddPC(APrisonerController* Controller)
{
	PlayerControllers.Add(Controller);
	Controller->ProximityChatComponent->JoinProximityChannel(GameChannelCode + "_" + ChannelProximityChatSuffix);

	//Mascot
	AMascot* NewMascotComponent = GetWorld()->SpawnActor<AMascot>(AMascot::StaticClass());

	if (NewMascotComponent)
	{
		if (Mascots.Contains(Controller))
			Mascots[Controller] = NewMascotComponent;
		else
			Mascots.Add(Controller, NewMascotComponent);
		NewMascotComponent->Init(MascotDatas[FMath::RandRange(0, MascotDatas.Num() - 1)], Controller);
	}

	for (auto PC : PlayerControllers)
	{
		APrisoner* Prisoner = Cast<APrisoner>(PC->GetPawn());
		if (!Prisoner) continue;
		Prisoner->Client_My_RefreshPlayerName();
	}

	if (!GameHasBegun && PlayerControllers.Num() == Cast<UOnlineGameInstance>(GetWorld()->GetGameInstance())->
		GetNbPlayersNeedToPlay())
	{
		OnGameReadyToStart.Broadcast();
	}
}

void AOnlineGameMode::RemovePC(APrisonerController* controller)
{
	PlayerControllers.Remove(controller);
}

TWeakObjectPtr<APrisonerController> AOnlineGameMode::GetNextOrPrevPC(TWeakObjectPtr<APrisonerController> Current,
                                                                     bool IsNext)
{
	int Index = PlayerControllers.Find(Current.Get());
	(IsNext) ? Index++ : Index--;
	if (Index < 0) Index = PlayerControllers.Num() - 1;
	if (Index >= PlayerControllers.Num()) Index = 0;

	return PlayerControllers[Index];
}

TWeakObjectPtr<APrisonerController> AOnlineGameMode::GetNextOrPrevAlivePC(
	TWeakObjectPtr<APrisonerController> Current, bool IsNext)
{
	TWeakObjectPtr<APrisonerController> Result = nullptr;
	int Index = PlayerControllers.Find(Current.Get());
	(IsNext) ? Index++ : Index--;
	if (Index < 0) Index = PlayerControllers.Num() - 1;
	if (Index >= PlayerControllers.Num()) Index = 0;

	Result = PlayerControllers[Index];
	AOnlinePlayerState* PlayerState = Result->GetPlayerState<AOnlinePlayerState>();
	if (!PlayerState) return nullptr;
	while (UFunctionsUtilities::BinaryMask_HaveValue(PlayerState->PlayerStates, StaticCast<int32>(EPlayerStates::Dead))
		||
		UFunctionsUtilities::BinaryMask_HaveValue(PlayerState->PlayerStates,
		                                          StaticCast<int32>(EPlayerStates::Spectator)))
	{
		(IsNext) ? Index++ : Index--;
		if (Index < 0) Index = PlayerControllers.Num() - 1;
		if (Index >= PlayerControllers.Num()) Index = 0;
		Result = PlayerControllers[Index];
		if (Result == Current) return nullptr;
		PlayerState = Result->GetPlayerState<AOnlinePlayerState>();
	}
	return Result;
}

void AOnlineGameMode::AskUpdateScore(int Score, int PlayerId, bool Forced)
{
	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		if (PlayerStates[i]->GetPlayerId() == PlayerId)
		{
			if (Forced)
				PlayerStates[i]->CurrentScore = Score;
			else
				PlayerStates[i]->CurrentScore += Score;
			PlayerStates[i]->OnRep_CurrentScore();
			CheckLeaderboard();
			break;
		}
	}
}

void AOnlineGameMode::AskUpdatePlayerState(EPlayerStates PlayerState, int PlayerId, bool PlayerStateValue)
{
	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		AOnlinePlayerState* CurrentPlayerState = PlayerStates[i];
		if (CurrentPlayerState && CurrentPlayerState->GetPlayerId() == PlayerId)
		{
			UFunctionsUtilities::BinaryMask_SetValue(CurrentPlayerState->PlayerStates, StaticCast<int32>(PlayerState),
			                                         PlayerStateValue);
			PlayerStates[i]->OnRep_PlayerStates();

			break;
		}
	}
}

int AOnlineGameMode::GetScore(int PlayerId)
{
	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		if (PlayerStates[i]->GetPlayerId() == PlayerId)
		{
			return PlayerStates[i]->CurrentScore;
		}
	}
	return 0;
}


bool AOnlineGameMode::CheckIsAllDeadOrSpectator()
{
	//TODO:ici
	bool IsAllDeadOrFinished = true;
	for (auto PlayerState : PlayerStates)
	{
		if (!PlayerState) continue;
		if (UFunctionsUtilities::BinaryMask_HaveValue(PlayerState->PlayerStates, StaticCast<int32>(EPlayerStates::Dead))
			||
			UFunctionsUtilities::BinaryMask_HaveValue(PlayerState->PlayerStates,
			                                          StaticCast<int32>(EPlayerStates::Spectator)))
		{
			IsAllDeadOrFinished = false;
			break;
		}
	}
	return IsAllDeadOrFinished;
}

FString AOnlineGameMode::GetHostPlayerName() const
{
	return MyGameState->GetChannelName();
}

void AOnlineGameMode::GameOver()
{
	// for (auto PC : PlayerControllers)
	// {
	// 	Cast<APrisoner>(PC->GetPawn())->TryDiePlayer(nullptr);
	// }
	EndExperience(false);
	OnQuestsFinished.Broadcast(false);
}

void AOnlineGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentMainTimer > 0.0f)
	{
		CurrentMainTimer -= DeltaSeconds;
		if (CurrentMainTimer <= 0.0f)
		{
			if (CurrentPhasisIndex == PhasisCount - 1)
			{
				PreviousPhasisEnded.Broadcast(CurrentPhasisIndex);
			}
			else
			{
			StartNewPhasis();
			}

			//StartNewPhasis();
		}
		for (auto PC : PlayerControllers)
		{
			if (!PC) continue;
			PC->OnTickTimer(CurrentMainTimer);
		}
	}
}

void AOnlineGameMode::SetMainTimer(float Timer)
{
	OnTimerFinished.Clear();
	CurrentMainTimer = Timer;
}

void AOnlineGameMode::PrisonerTakeElevator(int Index)
{
	bool IsAllSucceed = true;
	for (auto PC : PlayerControllers)
	{
		AOnlinePlayerState* PlayerState = PC->GetPlayerState<AOnlinePlayerState>();
		if (PlayerState->GetPlayerId() == Index)
		{
			PlayerState->SucceedToEscape = true;
		}
		if (!PlayerState->SucceedToEscape) IsAllSucceed = false;
	}
	if (IsAllSucceed)
		EndExperience(true);
}

void AOnlineGameMode::OpenRoom(ERoomType RoomType)
{
	if (MyGameState->RoomsOpened.Contains(RoomType)) return;
	MyGameState->RoomsOpened.Add(RoomType);
	OnRoomOpened.Broadcast(RoomType);
}

void AOnlineGameMode::CloseRoom(ERoomType RoomType)
{
	if (!MyGameState->RoomsOpened.Contains(RoomType)) return;
	MyGameState->RoomsOpened.Remove(RoomType);
	OnRoomClosed.Broadcast(RoomType);
}

int AOnlineGameMode::GetPlayerCount() const
{
	return PlayerControllers.Num();
}

int AOnlineGameMode::GetPhasisCount() const
{
	return PhasisCount;
}

int AOnlineGameMode::GetCurrentPhasisIndex() const
{
	return CurrentPhasisIndex;
}

void AOnlineGameMode::CheckLeaderboard()
{
	MyGameState->Leaderboard.Empty();
	for (auto PlayerState : PlayerStates)
	{
		if (!PlayerState) continue;
		FPlayerScore PlayerScore;
		PlayerScore.PlayerId = PlayerState->GetPlayerId();
		PlayerScore.PlayerName = PlayerState->SteamPlayerName;
		PlayerScore.Score = PlayerState->CurrentScore;
		MyGameState->Leaderboard.Push(PlayerScore);
	}
	MyGameState->Leaderboard.Sort(&AOnlineGameMode::ComparePlayerScore);
	MyGameState->OnRep_LeaderboardChanged();
	for (auto PC : PlayerControllers)
	{
		if (!PC) continue;
		PC->Client_OnLeaderboardChanged(MyGameState->Leaderboard);
	}
}

bool AOnlineGameMode::ComparePlayerScore(const FPlayerScore& First, const FPlayerScore& Second)
{
	return First.Score >= Second.Score;
}

void AOnlineGameMode::QuestsFinished(bool IsSuccess)
{
	OnQuestsFinished.Broadcast(IsSuccess);
	if (IsSuccess)
	{
		StartNewPhasis();
	}
}


void AOnlineGameMode::Logout(AController* Exiting)
{
	APlayerState* PlayerState = Exiting->PlayerState;
	if (PlayerState && PlayerState->IsABot() == false)
	{
		if (PlayerState == GameState->PlayerArray[0])
		{
			PlayerControllers.Remove(Cast<APrisonerController>(Exiting));
			PlayerStates.Remove(Exiting->GetPlayerState<AOnlinePlayerState>());
			for (auto PC : PlayerControllers)
			{
				if (PC)
					PC->BackToMainMenu();
			}
		}
	}
	Super::Logout(Exiting);
}


void AOnlineGameMode::StartNewPhasis()
{
	if (Phasis.Num() > 0)
	{
		if (CurrentPhasis)
		{
			if (QuestManager.IsValid())
				QuestManager.Get()->EndOfQuests(false);
			if (CurrentPhasis->PhasisEndDescription.ToString().Len() > 0)
				for (auto PC : PlayerControllers)
					PC->GetMascotAvatar()->PrintLine(CurrentPhasis->PhasisEndDescription.ToString(), false);
			PreviousPhasisEnded.Broadcast(CurrentPhasisIndex);
		}
		CurrentPhasis = Phasis[0];
		if (CurrentPhasis)
		{
			if (DelayBeforeStartTimer > 0.0f)
			{
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOnlineGameMode::StartNewPhasis,
				                                       DelayBeforeStartTimer, false);
				DelayBeforeStartTimer = 0.0f;
				CurrentPhasis = nullptr;
				return;
			}
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (QuestManager.IsValid() && CurrentPhasis->QuestManagerData)
				{
					QuestManager.Get()->Init(CurrentPhasis->QuestManagerData);
					QuestManager->Server_GetQuestsDescription();
				}

				if (CurrentPhasis->PhasisDuration > 0.0f)
				{
					SetMainTimer(CurrentPhasis->PhasisDuration);
				}

				for (auto Room : CurrentPhasis->RoomsOpened)
				{
					OpenRoom(Room);
				}
				for (auto Room : CurrentPhasis->RoomsClosed)
				{
					CloseRoom(Room);
				}
				if (EntitiesManager.IsValid() && CurrentPhasis->EntitiesToSpawn.Num() > 0)
					EntitiesManager->InitSpawnEntity(CurrentPhasis->EntitiesToSpawn);


				
					for (auto PC : PlayerControllers)
						PC->GetMascotAvatar()->InitToNewPhasis(CurrentPhasis->PhasisStartDescription.ToString(),
						                                       Phasis[0]->PhasisDuration,
						                                       QuestManager.IsValid()
							                                       ? QuestManager->QuestsDescription.ToString()
							                                       : "", false);
				
			
				CurrentPhasisIndex++;
				NewPhasisBegan.Broadcast(CurrentPhasisIndex);
				Phasis.RemoveAt(0);
				if (CurrentPhasisIndex == PhasisCount - 1)
				{
					IsLastPhasis.Broadcast();
				}
			}, 0.1, false);
		}
	}
	else
	{
		GameOver();
	}
}

void AOnlineGameMode::RefreshPlayerColor(int PlayerId, FLinearColor Color)
{
	if (!MyGameState.IsValid()) return;
	MyGameState->RefreshPlayerColor(PlayerId, Color);
}

void AOnlineGameMode::AskDestroyEntity(AAICharacter* Character)
{
	EntitiesManager->DespawnEntity(Character);
}

void AOnlineGameMode::AskStartGame()
{
	if (!GameHasBegun)
	{
		GameHasBegun = true;
		for (int i = 0; i < PlayerControllers.Num(); i++)
		{
			AOnlinePlayerState* PlayerState = PlayerControllers[i]->GetPlayerState<AOnlinePlayerState>();
			PlayerStates.Push(PlayerState);

			UOnlineGameInstance* ServerInstance = Cast<UOnlineGameInstance>(GetGameInstance());
			if (!ServerInstance) continue;

			FString PlayerName = PlayerState->SteamPlayerName;

			AskUpdateScore(0, PlayerState->GetPlayerId(), true);
			PlayerControllers[i]->GameStarted();
		}
		if (QuestManager.IsValid())
		{
			QuestManager->OnQuestsFinished.AddDynamic(this, &AOnlineGameMode::QuestsFinished);
		}
		OnStartGame.Broadcast();
	}
	StartNewPhasis();
}

void AOnlineGameMode::AddTime(float Time)
{
	CurrentMainTimer += Time;
}

void AOnlineGameMode::EndExperience(bool WithSuccess)
{
	OnEndGame.Broadcast();
	EndGame(WithSuccess);
}

void AOnlineGameMode::EndGame(bool WithSuccess)
{
	if (EntitiesManager.IsValid())
		EntitiesManager->DestroyAllEntities();

	for (auto PC : PlayerControllers)
	{
		PC->EndGame = true;
		APrisoner* MyCh = Cast<APrisoner>(PC->GetPawn());
		if (!MyCh) continue;
		MyCh->EndOfGame = true;
		AOnlinePlayerState* PlayerState = PC->GetPlayerState<AOnlinePlayerState>();
		PlayerState->SucceedToEscape = true;
		AskUpdatePlayerState(EPlayerStates::Spectator, PlayerState->GetPlayerId(), true);

		PC->WinnerName = MyGameState->Leaderboard[0].PlayerName;
		PC->Client_PrintWinner(WithSuccess);
	}
}
