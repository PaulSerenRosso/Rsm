// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerController.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Prisoner.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/InputAction.h"
#include "InputCoreTypes.h"
#include "ProjectEscapeLocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/OnlinePlayerState.h"
#include "ProjectEscapeProto/ActorComponents/SpectatorComponent.h"
#include "ProjectEscapeProto/DataAssets/MascotQuotesData.h"
#include "ProjectEscapeProto/DataAssets/PrisonnerInputDataConfig.h"
#include "ProjectEscapeProto/Mascot/MascotAvatar.h"
#include "ProjectEscapeProto/Portals/PortalManager.h"
#include "ProjectEscapeProto/Quests/QuestManager.h"
#include "ProjectEscapeProto/Quests/TaskSlot.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"

APrisonerController::APrisonerController()
{
	PrimaryActorTick.bCanEverTick = true;
	SpectatorComponent = nullptr;
	SpectatorComponent = CreateDefaultSubobject<USpectatorComponent>(TEXT("SpectatorComponent"));
	ProximityChatComponent = CreateDefaultSubobject<UProximityChat>(TEXT("ProximityChatComponent"));
}

void APrisonerController::BeginPlay()
{
	Super::BeginPlay();

	MySubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!MySubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No Enhanced Input Subsystem found"));
		return;
	}
	MySubsystem->ClearAllMappings();
	MySubsystem->AddMappingContext(MyInputMapping, 0);

	FActorSpawnParameters SpawnParameters;
	LocalPlayer = Cast<UProjectEscapeLocalPlayer>(GetLocalPlayer());
	if (IsLocalPlayerController())
	{
		ActorsSubcribedToStartGameEvent.Push(GetCharacter());
		OnActorSubscribedToGameStartEventInitialised.AddDynamic(
			this, &APrisonerController::OnActorSubscribedToGameStartEvent);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			ForceInitCharacter();
		}, 0.1f, false);
	}
}

void APrisonerController::InitInput(UInputComponent* PlayerInputComponent)
{
	this->MyPlayerInputComponent = PlayerInputComponent;
	MyCh = Cast<APrisoner>(GetPawn());
	if (!IsLocalController()) return;

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(InputDataConfig->LookAction, ETriggerEvent::Triggered, this, &APrisonerController::Look);
	Input->BindAction(InputDataConfig->MoveAction, ETriggerEvent::Triggered, this, &APrisonerController::MoveF);
	Input->BindAction(InputDataConfig->JumpAction, ETriggerEvent::Triggered, this, &APrisonerController::JumpF);
	Input->BindAction(InputDataConfig->JumpAction, ETriggerEvent::Completed, this,
	                  &APrisonerController::StopJumpingF);
	Input->BindAction(InputDataConfig->CrouchAction, ETriggerEvent::Started, this, &APrisonerController::CrouchF);
	Input->BindAction(InputDataConfig->CrouchAction, ETriggerEvent::Completed, this,
	                  &APrisonerController::StopCrouchF);
	Input->BindAction(InputDataConfig->SprintAction, ETriggerEvent::Started, this, &APrisonerController::SprintF);
	Input->BindAction(InputDataConfig->SprintAction, ETriggerEvent::Completed, this,
	                  &APrisonerController::StopSprintF);
	Input->BindAction(InputDataConfig->WalkAction, ETriggerEvent::Started, this,
	                  &APrisonerController::WalkQuietF);
	Input->BindAction(InputDataConfig->WalkAction, ETriggerEvent::Completed, this,
	                  &APrisonerController::StopWalkQuietF);
	Input->BindAction(InputDataConfig->PushAction, ETriggerEvent::Started, this,
	                  &APrisonerController::PushF);
	Input->BindAction(InputDataConfig->InteractAction, ETriggerEvent::Started, this,
	                  &APrisonerController::InteractF);
	Input->BindAction(InputDataConfig->InteractAction, ETriggerEvent::Completed, this,
	                  &APrisonerController::StopInteractF);
	Input->BindAction(InputDataConfig->DropAction, ETriggerEvent::Started, this, &APrisonerController::DropItem);
	Input->BindAction(InputDataConfig->UseItemAction, ETriggerEvent::Started, this, &APrisonerController::UseItem);
	Input->BindAction(InputDataConfig->UseItemAction, ETriggerEvent::Completed, this,
	                  &APrisonerController::EndUsingItem);
	Input->BindAction(InputDataConfig->SwapItemAction, ETriggerEvent::Started, this,
	                  &APrisonerController::SwapSlotInventory);
	Input->BindAction(InputDataConfig->MenuOptionsAction, ETriggerEvent::Started, this,
	                  &APrisonerController::OpenCloseMenuOptions);
	Input->BindAction(InputDataConfig->SwitchPrevCamAction, ETriggerEvent::Started, this,
	                  &APrisonerController::SwitchCamera, false);
	Input->BindAction(InputDataConfig->SwitchNextCamAction, ETriggerEvent::Started, this,
	                  &APrisonerController::SwitchCamera, true);
	Input->BindAction(InputDataConfig->PrintQuestAction, ETriggerEvent::Started, this,
	                  &APrisonerController::PrintQuest);
}


void APrisonerController::ChangeCam(APawn* NewPawn)
{
	SetViewTargetWithBlend(NewPawn, 0.01f, EViewTargetBlendFunction::VTBlend_Cubic);
}


void APrisonerController::SwitchCamera(bool Next)
{
	SpectatorComponent->SwitchCamera(Next, this);
}

void APrisonerController::ForceInitCharacter()
{
	FTimerHandle TimerHandle;
	UWorld* World = GetWorld();
	if (!World) return;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		APrisoner* MyCharacter = Cast<APrisoner>(GetPawn());
		if (!MyCharacter)
		{
			ForceInitCharacter();
			return;
		}
		MyCharacter->InitCharacterOnBP();
	}, 0.1f, false);
}

void APrisonerController::OnActorSubscribedToGameStartEvent(AActor* NewActor)
{
	if (ActorsSubcribedToStartGameEvent.Contains(NewActor))
	{
		ActorsSubcribedToStartGameEvent.Remove(NewActor);
		if (ActorsSubcribedToStartGameEvent.Num() == 0)
		{
			ProximityChatComponent->Init(IsLocalController());
			AddControllerToGameMode();
			OnlinePlayerState->OnPlayerStatesChanged.AddDynamic(this, &APrisonerController::OnPlayerStatesChanged);
		}
	}
}

void APrisonerController::BackToMainMenu_Implementation()
{
	ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void APrisonerController::OnTickTimer_Implementation(float Time)
{
	OnTimerUpdate.Broadcast(Time);
}

void APrisonerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APrisonerController::AddControllerToGameMode_Implementation()
{
	if (!HasAuthority()) return;
	AOnlineGameMode* GameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	GameMode->AddPC(this);
}

bool APrisonerController::AddControllerToGameMode_Validate()
{
	return true;
}


void APrisonerController::OnPlayerStatesChanged(int32 OldPlayerStates, int32 PlayerStates)
{
	if (!UFunctionsUtilities::BinaryMask_EqualValue(OldPlayerStates, PlayerStates,
	                                                StaticCast<int32>(EPlayerStates::Dead)))
	{
		if (UFunctionsUtilities::BinaryMask_HaveValue(PlayerStates, StaticCast<int32>(EPlayerStates::Dead)) && !EndGame)
		{
			DetachController();
			OnPlayerDead();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeadSound, GetPawn()->GetActorLocation(),
			                                      FRotator::ZeroRotator);
		}
		else
		{
			ReattachController();
			OnPlayerRespawn();
		}
	}
	else if (!UFunctionsUtilities::BinaryMask_EqualValue(OldPlayerStates, PlayerStates,
	                                                     StaticCast<int32>(EPlayerStates::Spectator)))
	{
		if (UFunctionsUtilities::BinaryMask_HaveValue(PlayerStates, StaticCast<int32>(EPlayerStates::Spectator)))
		{
			DetachController();
			if (APrisoner* MyCharacter = Cast<APrisoner>(GetPawn()))
				MyCharacter->DespawnCharacter(nullptr);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Player States changed aled"));
	}
}


int APrisonerController::GetPersonalPlayerID() const
    {
	APrisoner* MyCharacter = Cast<APrisoner>(GetPawn());
	if (!MyCharacter) return -1;
	return MyCharacter->GetPlayerState()->GetPlayerId();
}

void APrisonerController::GameStarted_Implementation()
{
	OnGameStarted.Broadcast();
}

void APrisonerController::Client_PrintWinner_Implementation(bool WithSuccess)
{
	PrintWinner(WithSuccess);
}

void APrisonerController::EndOfSlot()
{
	Client_EndOfSlot();
}

void APrisonerController::Client_EndOfSlot_Implementation()
{
	Server_EndOfSlot();
}

void APrisonerController::Server_EndOfSlot_Implementation()
{
	if (Slot)
	{
		Slot->EndOfSlot();
	}
}

void APrisonerController::SwapSlotInventory(const FInputActionValue& InputActionValue)
{
	MyCh->SwapSlotInventory();
}

FString APrisonerController::GetInputActionName(const UInputAction* InputAction) const
{
	const TArray<FEnhancedActionKeyMapping>& Mappings = MyInputMapping->GetMappings();
	for (const FEnhancedActionKeyMapping& Mapping : Mappings)
	{
		if (Mapping.Action == InputAction)
		{
			return Mapping.Key.GetDisplayName().ToString();
		}
	}
	return "";
}

void APrisonerController::DropItem(const FInputActionValue& InputActionValue)
{
	MyCh->DropItem();
}


void APrisonerController::Look(const FInputActionValue& Value)
{
	FVector2D Axis2DValue = Value.Get<FVector2D>();
	MyCh->Look(Axis2DValue);
}

void APrisonerController::MoveF(const FInputActionValue& Value)
{
	FVector2D Axis2DValue = Value.Get<FVector2D>();
	MyCh->Move(Axis2DValue);
}

void APrisonerController::JumpF(const FInputActionValue& Value)
{
	bool BoolValue = Value.Get<bool>();
	if (BoolValue)
	{
		MyCh->TryJump();
	}
}

void APrisonerController::StopJumpingF(const FInputActionValue& Value)
{
	bool BoolValue = Value.Get<bool>();
	if (BoolValue)
	{
		MyCh->StopJumping();
	}
}

void APrisonerController::CrouchF(const FInputActionValue& Value)
{
	MyCh->TryCrouch();
}

void APrisonerController::StopCrouchF(const FInputActionValue& Value)
{
	MyCh->StopCrouch();
}

void APrisonerController::SprintF(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		MyCh->Sprint();
	}
	else
	{
		MyCh->StopSprint();
	}
}

void APrisonerController::WalkQuietF(const FInputActionValue& InputActionValue)
{
	MyCh->WalkQuiet();
}

void APrisonerController::StopWalkQuietF(const FInputActionValue& InputActionValue)
{
	MyCh->StopWalkQuiet();
}

void APrisonerController::StopSprintF(const FInputActionValue& Value)
{
	MyCh->StopSprint();
}

void APrisonerController::InteractF(const FInputActionValue& Value)
{
	MyCh->PressInteract();
}

void APrisonerController::StopInteractF(const FInputActionValue& Value)
{
	MyCh->StopInteract();
}

void APrisonerController::PushF(const FInputActionValue& Value)
{
	MyCh->ServerPush();
}

void APrisonerController::UseItem(const FInputActionValue& Value)
{
	if (HasAuthority())
	{
		MyCh->MulticastUseItem();
	}
	else
	{
		MyCh->ServerUseItem();
	}
}

void APrisonerController::PrintQuest_Implementation(const FInputActionValue& Value)
{
	/*
	AQuestManager* QuestManager = Cast<AQuestManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AQuestManager::StaticClass()));
	if (!QuestManager) return;
	QuestManager->Server_GetQuestsDescription();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APrisonerController::PrintQuestInChat, 0.5f, false);*/
}

void APrisonerController::PrintQuestInChat()
{
	AQuestManager* MyQuestManager = Cast<AQuestManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AQuestManager::StaticClass()));
	if (!MyQuestManager) return;
	FText QuestsDescription = FText::Format(FText::FromString("{0}\n{1}"),
	                                        GetMascotAvatar()->MascotQuotesData->PrintQuestQuote,
	                                        MyQuestManager->QuestsDescription);
	GetMascotAvatar()->PrintLine(QuestsDescription.ToString(), true);
	//hollow
	HollowQuestItems();
}

void APrisonerController::EndUsingItem(const FInputActionValue& Value)
{
	if (HasAuthority())
	{
		MyCh->MulticastStopUsingItem();
	}
	else
	{
		MyCh->ServerStopUsingItem();
	}
}


void APrisonerController::DetachController()
{
	if (!IsLocalController()) return;
	MySubsystem->RemoveMappingContext(MyInputMapping);
	MySubsystem->AddMappingContext(MyInputGhostMapping, 0);
	SwitchCamera(true);
}

void APrisonerController::GoBackToBasicController(const UInputMappingContext* MappingContext)
{
	if (!MySubsystem) return;
	MySubsystem->RemoveMappingContext(MappingContext);
	MySubsystem->AddMappingContext(MyInputMapping, 0);
}

void APrisonerController::ReattachController()
{
	if (!IsLocalController()) return;
	GoBackToBasicController(MyInputGhostMapping);
	ChangeCam(GetPawn());
}

void APrisonerController::Client_OnLeaderboardChanged_Implementation(const TArray<FPlayerScore>& Leaderboard)
{
	BP_OnLeaderboardChanged(Leaderboard);
}

void APrisonerController::RSM_AddPoints(int Points)
{
	APrisoner* MyCharacter = Cast<APrisoner>(GetPawn());
	if (!MyCharacter) return;
	MyCharacter->AddPointToPlayer(Points);
}

void APrisonerController::RSM_UltimateCheat()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void APrisonerController::RSM_GodPlayer()
{
	APrisoner* MyCharacter = Cast<APrisoner>(GetPawn());
	if (!MyCharacter) return;
	MyCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	MyCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MyCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MyCharacter->MeshThirdPerson->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APrisonerController::RSM_SonicMode()
{
	APrisoner* MyCharacter = Cast<APrisoner>(GetPawn());
	if (!MyCharacter) return;
	MyCharacter->GetCharacterMovement()->MaxWalkSpeed = 10000.0f;
}

void APrisonerController::RSM_MoreTime(float Time)
{
	if (!HasAuthority()) return;
	AOnlineGameMode* GameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	GameMode->AddTime(Time);
}

void APrisonerController::RSM_NextPhase()
{
	if (!HasAuthority()) return;
	AOnlineGameMode* GameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	GameMode->StartNewPhasis();
}

void APrisonerController::OnPlayerPressInteract()
{
	if (!IsLocalController()) return;
	int PlayerId = GetPersonalPlayerID();

	Server_OnPlayerPressInteract(PlayerId);
}

void APrisonerController::Server_OnPlayerPressInteract_Implementation(int PlayerIdLocal)
{
	if (Slot)
	{
		Slot->OnPlayerPressInteract();
	}
}

void APrisonerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APrisonerController, SpectatorComponent);
	DOREPLIFETIME(APrisonerController, Slot);
	DOREPLIFETIME(APrisonerController, EndGame);
}


FMatrix APrisonerController::GetCameraProjectionMatrix() const
{
	FMatrix ProjectionMatrix;

	if (GetLocalPlayer() != nullptr)
	{
		FSceneViewProjectionData PlayerProjectionData;
		GetLocalPlayer()->GetProjectionData(GetLocalPlayer()->ViewportClient->Viewport,
		                                    PlayerProjectionData, static_cast<int32>(EStereoscopicPass::eSSP_FULL));

		ProjectionMatrix = PlayerProjectionData.ProjectionMatrix;
	}

	return ProjectionMatrix;
}

void APrisonerController::PerformCameraCut()
{
	if (LocalPlayer != nullptr)
	{
		LocalPlayer->PerformCameraCut();
	}
}
