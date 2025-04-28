// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectEscapeProto/OnlinePlayerState.h"
#include "ProjectEscapeProto/ActorComponents/ProximityChat.h"
#include "PrisonerController.generated.h"

struct FPlayerScore;
class ATaskSlot;
class UMascotAvatar;
class USpectatorComponent;
class UEnhancedInputLocalPlayerSubsystem;
class APortalManager;
struct FInputActionValue;
class UInputAction;
class APrisoner;
class UTweakeableValueCharacterData;
class UPrisonnerInputDataConfig;
class UInputMappingContext;
class UProjectEscapeLocalPlayer;
/**
 * 
 */
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorDelegate, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerDelegate, APrisonerController*, PC);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FtimerUpdate, float, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FColorUpdate, FLinearColor, NewColor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBlankDelegate);

UCLASS()
class PROJECTESCAPEPROTO_API APrisonerController : public APlayerController
{
	GENERATED_BODY()

public:
	APrisonerController();
	void InitInput(UInputComponent* PlayerInputComponent);
	virtual void BeginPlay() override;
	FString GetInputActionName(const UInputAction* Value) const;
	FMatrix GetCameraProjectionMatrix() const;
	void PerformCameraCut();
	void ChangeCam(APawn* NewPawn);
	void SwitchCamera(bool Next);
	USpectatorComponent* GetSpectatorComponent() const { return SpectatorComponent; }
	void ForceInitCharacter();
	
	UFUNCTION()
	void OnActorSubscribedToGameStartEvent(AActor* NewActor);
	UFUNCTION(Client, Reliable)
	void OnTickTimer(float Time);
	UFUNCTION(Client, Reliable)
	void BackToMainMenu();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetPersonalPlayerID() const;
	UFUNCTION(Client, Reliable)
	void Client_PrintWinner(bool WithSuccess);
	UFUNCTION(Client, Reliable)
	void GameStarted();
	void GoBackToBasicController(const UInputMappingContext* MappingContext);
	void ReattachController();
	UFUNCTION(BlueprintImplementableEvent)
	UMascotAvatar* GetMascotAvatar();
	UFUNCTION(Client, Reliable)
	void Client_OnLeaderboardChanged(const TArray<FPlayerScore>& Leaderboard);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnLeaderboardChanged(const TArray<FPlayerScore>& Leaderboard);
	
	///////////////////////
	UFUNCTION(Exec) // Works On Both Server and Client
	void RSM_AddPoints(int Points);
	UFUNCTION(Exec) //  Works On Both Server and Client
	void RSM_UltimateCheat();
	UFUNCTION(Exec) 
	void RSM_GodPlayer();
	UFUNCTION(Exec) // works Only on Server
	void RSM_SonicMode();
	UFUNCTION(Exec) // works Only on Server
	void RSM_MoreTime(float Time);
	UFUNCTION(Exec) // works Only on Server
	void RSM_NextPhase();
	
///////////////////////
	void OnPlayerPressInteract();
	UFUNCTION(Server, Reliable)
	void Server_OnPlayerPressInteract(int PlayerIdLocal);

	UPROPERTY(Replicated)
	ATaskSlot *Slot = nullptr;
	int PlayerIdSlot = -1;
//////////////////////

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PrintWinner(bool WithSuccess);
	UFUNCTION(BlueprintImplementableEvent)
	void OpenCloseMenuOptions(const FInputActionValue& Value);
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerDead();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerRespawn();
	UFUNCTION(BlueprintImplementableEvent)
	void HollowQuestItems();
	
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(Server, Reliable, WithValidation)
	void AddControllerToGameMode();
	
	UFUNCTION() 
	void OnPlayerStatesChanged(int32 OldPlayerStates, int32 PlayerStates);
	

	UFUNCTION(BlueprintCallable)
	void EndOfSlot();
	UFUNCTION(Client, Reliable)
	void Client_EndOfSlot();
	UFUNCTION(Server, Reliable)
	void Server_EndOfSlot();
	

private:
	void SwapSlotInventory(const FInputActionValue& InputActionValue);
	void DropItem(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& Value);
	void MoveF(const FInputActionValue& Value);
	void JumpF(const FInputActionValue& Value);
	void StopJumpingF(const FInputActionValue& Value);
	void CrouchF(const FInputActionValue& Value);
	void SprintF(const FInputActionValue& Value);
	void WalkQuietF(const FInputActionValue& InputActionValue);
	void StopWalkQuietF(const FInputActionValue& InputActionValue);
	void StopCrouchF(const FInputActionValue& Value);
	void StopSprintF(const FInputActionValue& Value);
	void InteractF(const FInputActionValue& Value);
	void StopInteractF(const FInputActionValue& Value);
	void PushF(const FInputActionValue& Value);
	void UseItem(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void PrintQuest(const FInputActionValue& Value);
	void PrintQuestInChat();
	void EndUsingItem(const FInputActionValue& Value);
	void DetachController();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;	

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UProximityChat* ProximityChatComponent;
	UPROPERTY(BlueprintReadWrite)
	AOnlinePlayerState* OnlinePlayerState = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UPrisonnerInputDataConfig* InputDataConfig;
	// UPROPERTY(BlueprintReadOnly)
	// APortalManager* PortalManager;
	UPROPERTY()
	UProjectEscapeLocalPlayer* LocalPlayer;
	UPROPERTY(BlueprintCallable, Category = "Events")
	FActorDelegate OnActorSubscribedToGameStartEventInitialised;
	UPROPERTY(BlueprintReadOnly)
	FString WinnerName;
	//FControllerDelegate OnPressInteract;
	UPROPERTY(VisibleAnywhere)
	UEnhancedInputLocalPlayerSubsystem *MySubsystem = nullptr;
	TWeakObjectPtr<UInputComponent> MyPlayerInputComponent = nullptr;
	UPROPERTY(Replicated)
	bool EndGame = false;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FColorUpdate OnColorUpdate;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* MyInputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* MyInputGhostMapping;
	UPROPERTY(Replicated)
	USpectatorComponent* SpectatorComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* DeadSound;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FtimerUpdate OnTimerUpdate;
	TArray<AActor*> ActorsSubcribedToStartGameEvent;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FBlankDelegate OnGameStarted;
	
private:
	TWeakObjectPtr<APrisoner> MyCh = nullptr;
};
