// Fill out your copyright notice in the Description page of Project Settings.


#include "Prisoner.h"
#include "EngineUtils.h"
#include "PrisonerController.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "ProjectEscapeProto/ActorComponents/InventorySystem.h"
#include "ProjectEscapeProto/DataAssets/TweakeableValueCharacterData.h"
#include "ProjectEscapeProto/Objects/Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/HubGameMode.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/CrouchActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/DropItemActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/InteractActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/JumpActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/LookActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/MoveActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/PushActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/SprintActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/SwitchItemActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/BaseActions/WalkActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionStateComponent.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Interactions/GrabItemActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/TemporaryEffects/TemporaryEffect.h"
#include "ProjectEscapeProto/Objects/Items/ScoreDrop.h"
#include "ProjectEscapeProto/Portals/PortalManager.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"
#include "ProjectEscapeProto/Utilities/MyBitReader.h"
#include "ProjectEscapeProto/Utilities/MyBitWriter.h"
#include "GameFramework/SpringArmComponent.h"

#include "ProjectEscapeProto/Quests/TaskSlot.h"
#include "ProjectEscapeProto/ActorComponents/StaminaSystem.h"


// Sets default values
APrisoner::APrisoner()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SoundEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("SoundEmitter"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Inventory = CreateDefaultSubobject<UInventorySystem>(TEXT("Inventory"));
	Stamina = CreateDefaultSubobject<UStaminaSystem>(TEXT("Stamina"));

	MeshThirdPerson = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	MeshThirdPerson->SetupAttachment(GetCapsuleComponent());

	//GetMesh()->SetupAttachment(CameraComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	TemporaryEffectComponent = CreateDefaultSubobject<UTemporaryEffect>(TEXT("TemporaryEffect"));
	TemporaryEffectComponent->SetupAttachment(RootComponent);

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(RootComponent);

	ChainsParent = CreateDefaultSubobject<USceneComponent>(TEXT("ChainsParent"));
	ChainsParent->SetupAttachment(MeshThirdPerson);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
}

void APrisoner::OnPushThing()
{
	Server_OnPushThing();
}

void APrisoner::Server_OnPushThing_Implementation()
{
	Multicast_OnPushThing();
}

void APrisoner::Multicast_OnPushThing_Implementation()
{
	OnPushAction.Broadcast();
}

void APrisoner::BeginPlay()
{
	Super::BeginPlay();
	SpringArm->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                             TEXT("CAMERA"));
	CameraComponent->AttachToComponent(SpringArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ChainsParent->AttachToComponent(MeshThirdPerson, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                                TEXT("Chains"));
	SetDefaultPlayerActionsReferences();
	UBinaryDataSubsystem* BinaryDataSubsystem = GetGameInstance()->GetSubsystem<UBinaryDataSubsystem>();
	MyBitReader = BinaryDataSubsystem->BitReader;
	MyBitWriter = BinaryDataSubsystem->BitWriter;
	SetDefaultAction(SprintComponent, USprintActionComponent::StaticClass());
	SetDefaultAction(MoveComponent, UMoveActionComponent::StaticClass());
	SetDefaultAction(CrouchComponent, UCrouchActionComponent::StaticClass());
	SetDefaultAction(WalkComponent, UWalkActionComponent::StaticClass());
	SetDefaultAction(JumpActionComponent, UJumpActionComponent::StaticClass());
	SetDefaultAction(LookActionComponent, ULookActionComponent::StaticClass());
	SetDefaultAction(InteractActionComponent, UInteractActionComponent::StaticClass());
	SetDefaultAction(SwitchItemActionComponent, USwitchItemActionComponent::StaticClass());
	SetDefaultAction(DropItemActionComponent, UDropItemActionComponent::StaticClass());
	SetDefaultAction(GrabItemActionComponent, UGrabItemActionComponent::StaticClass());
	SetDefaultAction(PushActionComponent, UPushActionComponent::StaticClass());

	if (IsLocallyControlled())
	{
		GetMesh()->SetVisibility(true, true);
		MeshThirdPerson->SetVisibility(false, true);
	}
	else
	{
		GetMesh()->SetVisibility(false, true);
		MeshThirdPerson->SetVisibility(true, true);
		SetDefaultAction(SprintComponent, USprintActionComponent::StaticClass());
	}
	if (HasAuthority())
	{
		Stamina->Server_InitStaminaSystem();
	}

	bUseControllerRotationPitch = false;
	CameraComponent->bUsePawnControlRotation = false;
	GetCharacterMovement()->JumpZVelocity = TweakeableData->JumpHeight;

	GetCharacterMovement()->JumpZVelocity = TweakeableData->JumpHeight;
	if (GetPlayerState() && !Cast<AOnlinePlayerState>(GetPlayerState()))
	{
		UE_LOG(LogTemp, Error, TEXT("No OnlinePlayerState found"));
		AOnlinePlayerState* NewPlayerState = GetWorld()->SpawnActor<AOnlinePlayerState>(
			AOnlinePlayerState::StaticClass());
		NewPlayerState->SetPlayerId(GetPlayerState()->GetPlayerId());
		NewPlayerState->SteamPlayerName = GetPlayerState()->GetPlayerName();
		SetPlayerState(NewPlayerState);
	}
	MoveToSpawnPoint();
	if (IsLocallyControlled())
		CheckEasterEggs();
}

void APrisoner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (UGameplayStatics::GetPlayerController(GetWorld(), 0) != nullptr)
	// {
	// 	APrisonerController* EPC = Cast<APrisonerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// 	EPC->PortalManager->Update(DeltaTime);
	// }


	if (!TextRenderComponent) return;

	APlayerController* LocalController = GetWorld()->GetFirstPlayerController();
	if (LocalController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		LocalController->GetPlayerViewPoint(CameraLocation, CameraRotation);


		FRotator LookAtRotation = (CameraLocation - TextRenderComponent->GetComponentLocation()).Rotation();
		LookAtRotation.Pitch = 0.0f;
		TextRenderComponent->SetWorldRotation(LookAtRotation);
	}

	FVector LocalPos = SpringArm->GetRelativeLocation();
	LocalPos.X = FMath::Clamp(LocalPos.X, -0.0f, 0.0f);
	SpringArm->SetRelativeLocation(LocalPos);
}

// Called to bind functionality to input
void APrisoner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APrisonerController* MyController = Cast<APrisonerController>(GetController());
	if (MyController)
	{
		MyController->InitInput(PlayerInputComponent);
	}
}

void APrisoner::UpdateScore_Implementation(int Score)
{
	AOnlineGameMode* GameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	GameMode->AskUpdateScore(Score, GetPlayerState()->GetPlayerId());
}

UCameraComponent* APrisoner::GetCameraComponent() const
{
	return CameraComponent;
}

void APrisoner::MulticastPush_Implementation()
{
	//Lancer l'animation de poussée
}

void APrisoner::ServerPush_Implementation()
{
	PushActionComponent->AskExecuteAction();
}

void APrisoner::ServerStopUsingItem_Implementation()
{
	MulticastStopUsingItem();
}

bool APrisoner::ServerStopUsingItem_Validate()
{
	return true;
}

void APrisoner::ServerUseItem_Implementation()
{
	MulticastUseItem();
}

bool APrisoner::ServerUseItem_Validate()
{
	return true;
}

void APrisoner::MulticastUseItem_Implementation()
{
	if (Inventory && Inventory->FirstItemSlot)
	{
		Inventory->FirstItemSlot->UseItem();
	}
}


void APrisoner::PressInteract()
{
	InteractActionComponent->AskExecuteAction();
}

void APrisoner::StopInteract()
{
	InteractActionComponent->AskStopExecuteAction();
}


void APrisoner::DropItem()
{
	DropItemActionComponent->AskExecuteAction();
}


void APrisoner::SwapSlotInventory()
{
	SwitchItemActionComponent->AskExecuteAction();
}

void APrisoner::SwitchToSpectator_Implementation()
{
	Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode())->AskUpdatePlayerState(
		EPlayerStates::Dead, GetPlayerState()->GetPlayerId(), true);
}

void APrisoner::TryDiePlayer_Implementation(AAICharacter* AICharacter)
{
	if (!AreAlive) return;
	AreAlive = false;
	OnCharacterDied.Broadcast(GetPlayerState()->GetPlayerId());
	DespawnCharacter(AICharacter);
	AOnlineGameMode* GM = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM) return;
	int CurrentScore = GM->GetScore(GetPlayerState()->GetPlayerId());
	int ScoreLost = CurrentScore * TweakeableData->PercentageCostToRespawn + TweakeableData->FlatCostToRespawn;
	if (CurrentScore - ScoreLost < 0)
		ScoreLost = CurrentScore;
	if (AScoreDrop* NewScoreDropped = Cast<AScoreDrop>(GetWorld()->SpawnActor(TweakeableData->ScoreDropActor)))
	{
		NewScoreDropped->SetActorLocation(GetActorLocation());
		NewScoreDropped->SetScore(ScoreLost);
	}
	Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode())->AskUpdateScore(
		CurrentScore - ScoreLost, GetPlayerState()->GetPlayerId(), true);

	TemporaryEffectComponent->StunCharacter(3);
	Inventory->DropInventory();
}


void APrisoner::MulticastStopUsingItem_Implementation()
{
	if (Inventory && Inventory->FirstItemSlot)
	{
		Inventory->FirstItemSlot->StopUsingItem();
	}
}

void APrisoner::GrabItem(AItem* Item)
{
	MyBitWriter->Init();
	GrabItemActionComponent->AskExecuteAction(MyBitWriter->GetData(), TArray<AActor*>({Item}));
}


FVector APrisoner::GetCameraPosition() const
{
	return CameraComponent->GetComponentLocation();
}

FVector APrisoner::GetCameraForwardVector() const
{
	return CameraComponent->GetForwardVector();
}

void APrisoner::MoveToSpawnPoint_Implementation()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (APlayerStart* PlayerStart = *It)
		{
			FVector Location = PlayerStart->GetActorLocation();
			FRotator Rotation = PlayerStart->GetActorRotation();

			SetActorLocationAndRotation(Location, Rotation, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
}

// void APrisoner::RespawnCharacter_Implementation()
// {
// 	AreAlive = true;
// 	Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode())->AskUpdatePlayerState(
// 		EPlayerStates::Dead, GetPlayerState()->GetPlayerId(), false);
// 	OnCharacterRespawn.Broadcast(GetPlayerState()->GetPlayerId());
// 	MoveToSpawnPoint();
// 	if (HasAuthority()) Multicast_RespawnCharacter();
// 	else Server_RespawnCharacter();
// }

void APrisoner::NewRespawnCharacter_Implementation(FTransform NewPoint)
{
	AreAlive = true;
	Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode())->AskUpdatePlayerState(
		EPlayerStates::Dead, GetPlayerState()->GetPlayerId(), false);
	OnCharacterRespawn.Broadcast(GetPlayerState()->GetPlayerId());
	//BPMoveToSpawnPoint(NewPoint);
	SetActorLocation(NewPoint.GetLocation());
	if (HasAuthority()) Multicast_RespawnCharacter();
	else Server_RespawnCharacter();
}


bool APrisoner::CanBePushed_Implementation(AActor* Actor)
{
	if (!TemporaryEffectComponent.IsValid()) return false;
	return !TemporaryEffectComponent->IsAlreadyPushed();
}

void APrisoner::AddPointToPlayer(int Points)
{
	Client_AddPointToPlayer(Points);
}

void APrisoner::Client_AddPointToPlayer_Implementation(int Points)
{
	int PlayerId = GetPlayerState()->GetPlayerId();
	Server_AddPointToPlayer(PlayerId, Points);
}

void APrisoner::Server_AddPointToPlayer_Implementation(int PlayerId, int Points)
{
	UFunctionsUtilities::Server_GivePointToPlayer(this, PlayerId, Points);
}

void APrisoner::OnPlayerStatesChanged(int32 OldPlayerStates, int32 PlayerStates)
{
	if (!UFunctionsUtilities::BinaryMask_EqualValue(OldPlayerStates, PlayerStates,
	                                                StaticCast<int32>(EPlayerStates::Dead)))
	{
		if (UFunctionsUtilities::BinaryMask_HaveValue(PlayerStates, StaticCast<int32>(EPlayerStates::Dead)))
		{
			OnCharacterDied.Broadcast(GetPlayerState()->GetPlayerId());
			if (HasAuthority())
			{
				Inventory->TryRemoveItem(Inventory->FirstItemSlot, this, true);
				Inventory->TryRemoveItem(Inventory->SecondItemSlot, this, false);
			}
		}
	}
}

void APrisoner::TryCrouch()
{
	CrouchComponent->AskExecuteAction();
}

void APrisoner::StopCrouch()
{
	CrouchComponent->AskStopAction();
}

void APrisoner::Sprint()
{
	SprintComponent->AskExecuteAction();
	OnRunning.Broadcast();
}

void APrisoner::WalkQuiet()
{
	WalkComponent->AskExecuteAction();
}

void APrisoner::StopWalkQuiet()
{
	WalkComponent->AskStopAction();
}

void APrisoner::StopSprint()
{
	SprintComponent->AskStopAction();
	OnStopRunning.Broadcast();
}

void APrisoner::Server_MyRefreshPlayerName_Implementation(const FString& NewName)
{
	Multicast_MyRefreshPlayerName(NewName);
}


void APrisoner::Multicast_MyRefreshPlayerName_Implementation(const FString& NewName)
{
	TextRenderComponent->SetText(FText::FromString(NewName));
}

void APrisoner::GenerateSound_Implementation(float NoiseEmitted)
{
	LastSoundEmitted = NoiseEmitted / MAX_NOISE;
	SoundEmitter->MakeNoise(this, NoiseEmitted, GetActorLocation());
	GetWorldTimerManager().ClearTimer(TimerHandleNoise);
	GetWorldTimerManager().SetTimer(TimerHandleNoise, this, &APrisoner::CallOutNoise, 0.1f,
	                                false);
}

bool APrisoner::GenerateSound_Validate(float NoiseEmitted)
{
	return true;
}

void APrisoner::Move(FVector2D Direction)
{
	MyBitWriter->Init();
	MyBitWriter->SetVector2D(Direction);
	MoveComponent->AskExecuteAction(MyBitWriter->GetData());
}

void APrisoner::TryJump()
{
	JumpActionComponent->AskExecuteAction();
}

void APrisoner::Look(FVector2D RotationVector)
{
	MyBitWriter->Init();
	MyBitWriter->SetVector2D(RotationVector);
	LookActionComponent->AskExecuteAction(MyBitWriter->GetData());
}

void APrisoner::DisableMovement()
{
	if (UCharacterMovementComponent* Movement = FindComponentByClass<UCharacterMovementComponent>())
	{
		Movement->DisableMovement();
	}
}

void APrisoner::EnableMovement()
{
	if (UCharacterMovementComponent* Movement = FindComponentByClass<UCharacterMovementComponent>())
	{
		Movement->SetMovementMode(MOVE_Walking);
	}
}

void APrisoner::Multicast_MoveToPos_Implementation(FVector NewLocation)
{
	SetActorLocation(NewLocation);
}

void APrisoner::Server_MoveToPos_Implementation(FVector NewLocation)
{
	SetActorLocation(NewLocation);
	Multicast_MoveToPos(NewLocation);
}

void APrisoner::SetDefaultAction(UPlayerActionComponent* NewAction, TSubclassOf<UPlayerActionComponent> Class)
{
	if (HasAuthority())
	{
		NewAction->InitServerAction(this);
	}
	else
	{
		NewAction->InitMulticastAction(this);
	}
	CurrentPlayerActions.Add(Class, NewAction);
}

void APrisoner::CheckEasterEggs()
{
	FString PlayerName = Cast<UOnlineGameInstance>(GetGameInstance())->GetSteamAccountName();
	Server_SpawnEasterEggs(PlayerName);
}

void APrisoner::Server_SpawnEasterEggs_Implementation(const FString& PlayerName)
{
	if (PlayerName == TEXT("MIAOU"))
	{
		FActorSpawnParameters SpawnInfo;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation() + GetCameraForwardVector() * 100);
		AActor* Hat = GetWorld()->SpawnActor(CatItem, &SpawnTransform, SpawnInfo);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this, Hat]()
		{
			USkeletalMeshComponent* TPV = MeshThirdPerson;
			Hat->AttachToComponent(TPV, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                       TEXT("Hat"));
			Client_SpawnEasterEggs();
		}, 0.2f, false);
	}
	else if (PlayerName == TEXT("Support"))
	{
		FActorSpawnParameters SpawnInfo;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation() + GetCameraForwardVector() * 100);
		GetWorld()->SpawnActor(SupportItem, &SpawnTransform, SpawnInfo);
	}
}


void APrisoner::Client_SpawnEasterEggs_Implementation()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (IsLocallyControlled())
		{
			GetMesh()->SetVisibility(true, true);
			MeshThirdPerson->SetVisibility(false, true);
		}
		else
		{
			GetMesh()->SetVisibility(false, true);
			MeshThirdPerson->SetVisibility(true, true);
			SetDefaultAction(SprintComponent, USprintActionComponent::StaticClass());
		}
	}, 0.001f, false);
}

void APrisoner::ForceStartGame()
{
	AHubGameMode* GameMode = Cast<AHubGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;
	GameMode->ForceStartGame();
}

void APrisoner::Cheat_GainPoints_Implementation(int Points)
{
	Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode())->AskUpdateScore(Points, GetPlayerState()->GetPlayerId());
}

void APrisoner::Cheat_EndOfGame_Implementation()
{
	Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode())->GameOver();
}

void APrisoner::Server_RemovePlayerActionComponent_Implementation(TSubclassOf<UPlayerActionComponent> Class)
{
	CurrentPlayerActions.Remove(Class);
}

void APrisoner::Server_AddPlayerActionComponent_Implementation(
	UPlayerActionComponent* PlayerActionStateComponent, TSubclassOf<UPlayerActionComponent> Class)
{
	CurrentPlayerActions.Add(Class, PlayerActionStateComponent);
	PlayerActionStateComponent->Server_InitAction(this);
}


UPlayerActionComponent* APrisoner::GetPlayerActionComponent(TSubclassOf<UPlayerActionComponent> Class)
{
	if (!HasAuthority()) return nullptr;
	if (CurrentPlayerActions.Contains(Class))
	{
		return CurrentPlayerActions[Class];
	}
	return nullptr;
}

bool APrisoner::TryPush_Implementation(float Duration, FVector Direction, float Force)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Try to Push Actor on Prisoner"));
	TemporaryEffectComponent->PushCharacter(Duration, Direction, Force);
	Client_Pushing();
	IsPushing.Broadcast();
	return IPushable::TryPush_Implementation(Duration, Direction, Force);
}

void APrisoner::Client_Pushing_Implementation()
{
	Client_IsPushing.Broadcast();
}

void APrisoner::Client_My_RefreshPlayerName_Implementation()
{
	UOnlineGameInstance* GM = Cast<UOnlineGameInstance>(GetGameInstance());
	if (!GM) return;
	FString NewName = GM->GetSteamAccountName();
	HasAuthority() ? Multicast_MyRefreshPlayerName(NewName) : Server_MyRefreshPlayerName(NewName);
}

void APrisoner::ChangeVisionOfWidget_Implementation(UWidgetComponent* Widget, bool IsVisible)
{
	Widget->SetHiddenInGame(!IsVisible);
}


void APrisoner::Multicast_RespawnCharacter_Implementation()
{
	if (GetMesh() && IsLocallyControlled())
	{
		GetMesh()->SetVisibility(true, true);
	}
	if (MeshThirdPerson && !IsLocallyControlled())
	{
		MeshThirdPerson->SetVisibility(true, true);
	}
}

void APrisoner::Server_RespawnCharacter_Implementation()
{
	if (EndOfGame) return;
	Multicast_RespawnCharacter();
}


void APrisoner::Multicast_DespawnCharacter_Implementation(AAICharacter* AICharacter)
{
	if (EndOfGame) return;
	BP_DespawnCharacter(AICharacter);
}

void APrisoner::Server_DespawnCharacter_Implementation(AAICharacter* AICharacter)
{
	Multicast_DespawnCharacter(AICharacter);
}

void APrisoner::DespawnCharacter(AAICharacter* AICharacter)
{
	if (HasAuthority()) Multicast_DespawnCharacter(AICharacter);
	else Server_DespawnCharacter(AICharacter);
}

void APrisoner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APrisoner, CanStartPlayersActions);
	DOREPLIFETIME(APrisoner, PlayerActionStateMask);
	DOREPLIFETIME(APrisoner, LastSoundEmitted);
	DOREPLIFETIME(APrisoner, EndOfGame);
}

void APrisoner::CallOutNoise()
{
	LastSoundEmitted = 0.0f;
}
