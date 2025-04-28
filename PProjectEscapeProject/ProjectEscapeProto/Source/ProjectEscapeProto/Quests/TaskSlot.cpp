#include "TaskSlot.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/Characters/PrisonerController.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"

DEFINE_LOG_CATEGORY_STATIC(LogTaskSlot, Log, All);

ATaskSlot::ATaskSlot()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Initialize Slot Position
	SlotPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SlotPosition"));
	if (ItemMesh)
	{
		SlotPosition->SetupAttachment(ItemMesh);
	}

	// Initialize Camera Pivot
	CameraPivot = CreateDefaultSubobject<UChildActorComponent>(TEXT("CameraPivot"));
	CameraPivot->SetupAttachment(SlotPosition);
	CameraPivot->SetChildActorClass(ACameraActor::StaticClass());

	// Default Exit Delay
	ExitDelay = 1.0f;
}

void ATaskSlot::EndOfSlot()
{
	IncompleteInteraction = false;
	OnEndSlot.Broadcast();
	//BP_EndOfSlot();
}

void ATaskSlot::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()->GetFirstPlayerController()->IsLocalController())
	{
		return;
	}

	// Validate Components
	if (!SlotPosition || !CameraPivot)
	{
		UE_LOG(LogTaskSlot, Error, TEXT("%s: SlotPosition or CameraPivot is not properly initialized."), *GetName());
		return;
	}
	if (!MyTaskSlotInputMapping) return;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		APrisonerController* PC = Cast<APrisonerController>(GetWorld()->GetFirstPlayerController());
		if (PC && PC->MyPlayerInputComponent.IsValid() && PC->IsLocalPlayerController())
		{
				if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PC->MyPlayerInputComponent))
				{
					// Nettoyer et réassigner les binds
					//InputComp->ClearActionBindings();
					const TArray<FEnhancedActionKeyMapping>& Mappings = MyTaskSlotInputMapping->GetMappings();
					Input->BindAction(MyTaskSlotInputMapping->GetMappings()[0].Action, ETriggerEvent::Started, PC,
					                  &APrisonerController::OnPlayerPressInteract);
					Input->BindAction(MyTaskSlotInputMapping->GetMappings()[1].Action, ETriggerEvent::Started, this,
					                  &ATaskSlot::StopInteract);
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InputComponent found!"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("InputComponent not found!"));
				}
		}
	}, 1.0f, false);
}

void ATaskSlot::StopInteract()
{
	if (IsMyself())
	{
		//ServerExitSlot();
		ExitSlot();
	}
}

bool ATaskSlot::IsMyself()
{
	APrisonerController* PC = Cast<APrisonerController>(GetWorld()->GetFirstPlayerController());
	if (!PC) return false;
	APrisoner* Prisoner = Cast<APrisoner>(PC->GetPawn());
	return Prisoner && OccupyingPrisoner == Prisoner->GetPlayerState()->GetPlayerId();
}

void ATaskSlot::OnPlayerPressInteract()
{
	if (!IncompleteInteraction) return;
	OnPlayerPressInteractBP.Broadcast();
}

void ATaskSlot::HandlePressInteract(APrisonerController* PC)
{
	// Handle interaction
	// if (IsMyself() && bCanExit)
	// {
	// 	ServerExitSlot();
	// }
}

bool ATaskSlot::CanInteract_Implementation(AActor* Actor)
{
	APrisoner* Prisoner = Cast<APrisoner>(Actor);
	return Prisoner && IncompleteInteraction && CanBeUsed && (OccupyingPrisoner < 0
		|| OccupyingPrisoner == Prisoner->GetPlayerState()->GetPlayerId() 
	);
}

bool ATaskSlot::TryInteract_Implementation(AActor* Actor)
{
	APrisoner* Prisoner = Cast<APrisoner>(Actor);
	if (!Prisoner) return false;

	Prisoner->OnCharacterDied.RemoveDynamic(this, &ATaskSlot::ExitSlotWithIdParameter);
	Prisoner->OnCharacterDied.AddDynamic(this, &ATaskSlot::ExitSlotWithIdParameter);
	Prisoner->IsPushing.RemoveDynamic(this, &ATaskSlot::ServerExitSlot);
	Prisoner->IsPushing.AddDynamic(this, &ATaskSlot::ServerExitSlot);

	if (OccupyingPrisoner < 0)
	{
		ServerEnterSlot(Prisoner);
		return true;
	}
	if (OccupyingPrisoner == Prisoner->GetPlayerState()->GetPlayerId())
	{
		ServerExitSlot();
		return true;
	}
	Prisoner->OnCharacterDied.RemoveDynamic(this, &ATaskSlot::ExitSlotWithIdParameter);
	Prisoner->IsPushing.RemoveDynamic(this, &ATaskSlot::ServerExitSlot);

	return false;
}

void ATaskSlot::ExitSlotWithIdParameter(int PlayerId)
{
	ServerExitSlot();
}

// void ATaskSlot::ForceExitSlot(int PlayerId)
// {
// }

void ATaskSlot::ChangeFloatValue(float NewValue)
{
	UE_LOG(LogTaskSlot, Log, TEXT("%s: Value changed to %f."), *GetName(), NewValue);
	if (OccupyingPrisoner < 0) return;
	APrisoner* Prisoner = UFunctionsUtilities::Server_GetPlayerWithId(this, OccupyingPrisoner);
}

void ATaskSlot::ServerChangeFloatValue_Implementation(float NewValue)
{
	UE_LOG(LogTaskSlot, Log, TEXT("%s: Value changed On server to %f."), *GetName(), NewValue);
	TaskValue = NewValue;
}

bool ATaskSlot::ServerEnterSlot_Validate(APrisoner* Prisoner)
{
	return IsValid(Prisoner);
}

void ATaskSlot::ServerEnterSlot_Implementation(APrisoner* Prisoner)
{
	if (!Prisoner) return;
	const FVector AdjustedLocation = SlotPosition->GetComponentLocation() + SlotPosition->GetForwardVector() * 100.0f +
		FVector(0.0f, 0.0f, 50.0f);


	Prisoner->SetActorLocation(AdjustedLocation, false);

	
	//Prisoner->GetController()->SetControlRotation(GetActorRotation() + FRotator(0.0f, -180.0f, 0.0f));
	Prisoner->GetController()->SetControlRotation(SlotPosition->GetComponentRotation() + FRotator(0.0f, 180.0f, 0.0f));
	if (OccupyingPrisoner >= 0)
	{
		UE_LOG(LogTaskSlot, Warning, TEXT("%s: Slot is already occupied."), *GetName());
		return;
	}
	OccupyingPrisoner = Prisoner->GetPlayerState()->GetPlayerId();

	APrisonerController* MyController = Cast<APrisonerController>(Prisoner->GetController());
	MyController->Slot = this;
	MyController->PlayerIdSlot = Prisoner->GetPlayerState()->GetPlayerId();

	MulticastEnterSlot(Prisoner);
}

void ATaskSlot::MulticastEnterSlot_Implementation(APrisoner* Prisoner)
{
	if (!Prisoner) return;
	if (!GetWorld()->GetFirstPlayerController()) return;
	if (!GetWorld()->GetFirstPlayerController()->GetPawn()) return;
	int LocalPlayerId = Cast<APrisoner>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetPlayerState()->
		GetPlayerId();

	if (!Prisoner || LocalPlayerId != Prisoner->GetPlayerState()->GetPlayerId())
	{
		UE_LOG(LogTaskSlot, Warning, TEXT("%s: Invalid Prisoner attempting to enter slot."), *GetName());
		return;
	}

	// Set the player's view to the CameraPivot
	APlayerController* PC = Cast<APlayerController>(Prisoner->GetController());
	if (PC && CameraPivot && CameraPivot->GetChildActor())
	{
		PC->SetControlRotation(SlotPosition->GetComponentRotation() + FRotator(0.0f, 180.0f, 0.0f));

		PC->SetViewTargetWithBlend(CameraPivot->GetChildActor(), 0.5f);

		TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> MySubsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (MySubsystem.IsValid() && Prisoner->IsAlive())
		{
			MySubsystem->ClearAllMappings();
			MySubsystem->AddMappingContext(MyTaskSlotInputMapping, 10);
			MySubsystem->RequestRebuildControlMappings();
			APrisonerController* MyController = Cast<APrisonerController>(PC);
			MyController->Slot = this;
		}

		OnPlayerEnterSlotBP.Broadcast();
	}

	// Play appropriate animation
	USkeletalMeshComponent* PrisonerMesh = Prisoner->GetMesh();
	if (Prisoner->IsLocallyControlled() && SlotAnim1P && PrisonerMesh)
	{
		//PrisonerMesh->PlayAnimation(SlotAnim1P, true);
	}
	else if (SlotAnim3P && PrisonerMesh)
	{
		PrisonerMesh->PlayAnimation(SlotAnim3P, true);
	}

	// Trigger the Blueprint event
	OnEnterSlot(Prisoner);

	// Update slot state
	
	// Enable exit after delay
	GetWorld()->GetTimerManager().SetTimer(ExitDelayHandle, this, &ATaskSlot::EnableExit, ExitDelay, false);
}

bool ATaskSlot::ServerExitSlot_Validate()
{
	return true;
}

void ATaskSlot::ServerExitSlot_Implementation()
{
	IncompleteInteraction = true;
	ServerNotExitSlot();
	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		OccupyingPrisoner = -1;
	}, 0.2f, false);
}

void ATaskSlot::ServerNotExitSlot()
{
	APrisoner* Player = UFunctionsUtilities::Server_GetPlayerWithId(this, OccupyingPrisoner);
	if (!Player) return;
	Player->OnCharacterDied.RemoveDynamic(this, &ATaskSlot::ForceExitSlot);
	Player->IsPushing.RemoveDynamic(this, &ATaskSlot::ServerExitSlot);
	MulticastExitSlot();
}

void ATaskSlot::MulticastExitSlot_Implementation()
{
	ExitSlot();
}


void ATaskSlot::SERVER_InteractOnServer_Implementation()
{
	OnPlayerPressInteract();
}

bool ATaskSlot::SERVER_InteractOnServer_Validate()
{
	return true;
}

void ATaskSlot::ExitSlot()
{
	if (OccupyingPrisoner < 0) return;

	APrisoner* Prisoner = Cast<APrisoner>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// Reset the player's view and movement
	APrisonerController* PC = Cast<APrisonerController>(Prisoner->GetController());
	if (!PC) return;

	PC->SetViewTargetWithBlend(Prisoner, 0.5f);

	if (Prisoner->GetMesh())
	{
		Prisoner->GetMesh()->Stop();
	}

	//Prisoner->EnableMovement();
	if (PC && Prisoner->IsAlive())
		PC->GoBackToBasicController(MyTaskSlotInputMapping);

	// Trigger the Blueprint event
	OnExitSlot(Prisoner);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		OccupyingPrisoner = -1;
	}, 0.2f, false);

	//OccupyingPrisoner = -1;
	OnPlayerExitSlotBP.Broadcast();
	UE_LOG(LogTaskSlot, Log, TEXT("%s: Prisoner exited slot successfully."), *GetName());
}

void ATaskSlot::EnableExit()
{
	UE_LOG(LogTaskSlot, Log, TEXT("%s: Exit enabled."), *GetName());
}

void ATaskSlot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATaskSlot, OccupyingPrisoner);
}
