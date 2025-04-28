// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectEscapeProto/OnlineGameInstance.h"
#include "ProjectEscapeProto/Interfaces/Pushable.h"
#include "Prisoner.generated.h"

class AAICharacter;
class ATaskSlot;
class UStaminaSystem;
class UTextRenderComponent;
class UPushActionComponent;
class UTemporaryEffect;
class UInteractActionComponent;
class UJumpActionComponent;
class ULookActionComponent;
class UGrabItemActionComponent;
class USwitchItemActionComponent;
class UDropItemActionComponent;
class USprintActionComponent;
class UWalkActionComponent;
class UMyBitWriter;
class UMyTemplateVector2D;
class UMyTemplateFloat;
class UMyTemplate;
class UCrouchActionComponent;
class UMoveActionComponent;
class UPlayerActionComponent;
class UPlayerActionStateComponent;
class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UInventorySystem;
class UTweakeableValueCharacterData;
class UPrisonnerInputDataConfig;
class UInputMappingContext;
class UInputAction;
class USpringArm;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, int32, PlayerId);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBlankPrisonerParamDelegate);


UCLASS()
class PROJECTESCAPEPROTO_API APrisoner : public ACharacter, public IPushable
{
	GENERATED_BODY()

public:
	APrisoner();

	UCameraComponent SetupAttachment(USpringArm);
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(Server, Reliable, WithValidation)
	void GenerateSound(float NoiseEmitted);

	void PressInteract();
	void StopInteract();


	void DropItem();
	void SwapSlotInventory();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryDiePlayer(AAICharacter* AICharacter);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SwitchToSpectator();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UpdateScore(int Score);
	UCameraComponent* GetCameraComponent() const;
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUseItem();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastUseItem();
	UFUNCTION(Server, Reliable)
	void ServerPush();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPush();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopUsingItem();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopUsingItem();
	bool IsAlive() const { return AreAlive; }
	void GrabItem(AItem* Item);
	UFUNCTION(BlueprintImplementableEvent)
	void InitCharacterOnBP();
	FVector GetCameraPosition() const;
	FVector GetCameraForwardVector() const;
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void MoveToSpawnPoint();
	UFUNCTION(BlueprintImplementableEvent)
	void BPMoveToSpawnPoint(FTransform NewTransform);
	// UFUNCTION(BlueprintCallable, Server, Reliable)
	// void RespawnCharacter();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void NewRespawnCharacter(FTransform NewPoint);
	virtual bool CanBePushed_Implementation(AActor* Actor) override;
	UFUNCTION(BlueprintCallable)
	void AddPointToPlayer(int Points);
	UFUNCTION(Client, Reliable)
	void Client_AddPointToPlayer(int Points);
	UFUNCTION(Server, Reliable)
	void Server_AddPointToPlayer(int PlayerId, int Points);

	UFUNCTION()
	void OnPlayerStatesChanged(int32 OldPlayerStates, int32 PlayerStates);

	void Move(const FVector2D Direction);
	void TryCrouch();
	void Sprint();
	void WalkQuiet();
	void StopWalkQuiet();
	void StopCrouch();
	void StopSprint();
	void TryJump();
	void Look(FVector2D RotationVector);

	// Pour les slots
	void DisableMovement();
	void EnableMovement();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_MoveToPos(FVector NewLocation);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Multicast_MoveToPos(FVector NewLocation);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ChangeVisionOfWidget(UWidgetComponent* Widget, bool IsVisible);


	void DespawnCharacter(AAICharacter* AICharacter);

	UFUNCTION(Server, Reliable)
	void Server_AddPlayerActionComponent(UPlayerActionComponent* PlayerActionStateComponent,
	                                     TSubclassOf<UPlayerActionComponent> Class);
	UFUNCTION(Server, Reliable)
	void Server_RemovePlayerActionComponent(TSubclassOf<UPlayerActionComponent> Class);
	UPlayerActionComponent* GetPlayerActionComponent(TSubclassOf<UPlayerActionComponent> Class);

	virtual bool TryPush_Implementation(float Duration, FVector Direction, float Force) override;
	UFUNCTION(Client, Reliable)
	void Client_Pushing();

	UFUNCTION(Client, Reliable)
	void Client_My_RefreshPlayerName();
	UFUNCTION(Server, Reliable)
	void Server_MyRefreshPlayerName(const FString& NewName);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MyRefreshPlayerName(const FString& NewName);
	UFUNCTION(BlueprintImplementableEvent)
	void SwapHintPrevisualisationBP(bool Enabled);
	void OnPushThing();
	UFUNCTION(Server, Reliable)
	void Server_OnPushThing();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnPushThing();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTemporaryEffect* GetTemporaryEffectComponent() const { return TemporaryEffectComponent.Get(); }

protected:
	virtual void BeginPlay() override;

	void PlayWalkSound();

	UFUNCTION(Server, Reliable)
	void Server_DespawnCharacter(AAICharacter* AICharacter);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DespawnCharacter(AAICharacter* AICharacter);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_DespawnCharacter(AAICharacter* AICharacter);
	UFUNCTION(Server, Reliable)
	void Server_RespawnCharacter();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RespawnCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void SetDefaultAction(UPlayerActionComponent* NewAction, TSubclassOf<UPlayerActionComponent> Class);
	UFUNCTION(BlueprintImplementableEvent)
	void SetDefaultPlayerActionsReferences();

	void CheckEasterEggs();
	UFUNCTION(Server, Reliable)
	void Server_SpawnEasterEggs(const FString &Name);
	UFUNCTION(Client, Reliable)
	void Client_SpawnEasterEggs();
	
	/////////////////////////CHEATS/////////////////////////
	UFUNCTION(BlueprintCallable)
	void ForceStartGame();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Cheat_GainPoints(int Points);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Cheat_EndOfGame();
	///////////////////////////////////////////////////////
	///

private:
	void CallOutNoise();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	UTweakeableValueCharacterData* TweakeableData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	USkeletalMeshComponent* MeshThirdPerson;
	UPROPERTY(BlueprintReadOnly, Category = "Sound")
	UPawnNoiseEmitterComponent* SoundEmitter;
	FCharacterDiedDelegate OnCharacterDied;
	FCharacterDiedDelegate OnCharacterRespawn;

	UPROPERTY(BlueprintAssignable)
	FBlankPrisonerParamDelegate OnRunning;
	UPROPERTY(BlueprintAssignable)
	FBlankPrisonerParamDelegate OnStopRunning;
	UPROPERTY(BlueprintAssignable)
	FBlankPrisonerParamDelegate IsPushing;
	UPROPERTY(BlueprintAssignable)
	FBlankPrisonerParamDelegate Client_IsPushing;
	UPROPERTY(BlueprintAssignable)
	FBlankPrisonerParamDelegate OnPushAction;


	UPROPERTY(Replicated)
	bool CanStartPlayersActions = true;
	UPROPERTY(Replicated, BlueprintReadWrite, meta=(Bitmask, BitmaskEnum = "EPlayerActionState"))
	int32 PlayerActionStateMask;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventorySystem* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UStaminaSystem* Stamina;
	TMap<TSubclassOf<UPlayerActionComponent>, UPlayerActionComponent*> CurrentPlayerActions;
	TWeakObjectPtr<UTemporaryEffect> TemporaryEffectComponent = nullptr;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Data")
	bool EndOfGame = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComponent;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Sound")
	float LastSoundEmitted = 0.f;

	UPROPERTY(BlueprintReadWrite)
	UAudioComponent* AudioComponent;
	UPROPERTY(BlueprintReadWrite)
	UMoveActionComponent* MoveComponent;
	UPROPERTY(BlueprintReadWrite)
	UCrouchActionComponent* CrouchComponent;
	UPROPERTY(BlueprintReadWrite)
	UWalkActionComponent* WalkComponent;
	UPROPERTY(BlueprintReadWrite)
	USprintActionComponent* SprintComponent;
	UPROPERTY(BlueprintReadWrite)
	UDropItemActionComponent* DropItemActionComponent;
	UPROPERTY(BlueprintReadWrite)
	USwitchItemActionComponent* SwitchItemActionComponent;
	UPROPERTY(BlueprintReadWrite)
	UGrabItemActionComponent* GrabItemActionComponent;
	UPROPERTY(BlueprintReadWrite)
	ULookActionComponent* LookActionComponent;
	UPROPERTY(BlueprintReadWrite)
	UJumpActionComponent* JumpActionComponent;
	UPROPERTY(BlueprintReadWrite)
	UInteractActionComponent* InteractActionComponent;
	UPROPERTY(BlueprintReadWrite)
	UPushActionComponent* PushActionComponent;
	TWeakObjectPtr<UMyBitWriter> MyBitWriter;
	TWeakObjectPtr<UMyBitReader> MyBitReader;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UTextRenderComponent* TextRenderComponent;
	UPROPERTY(EditAnywhere, Category = "Data")
	USceneComponent* ChainsParent;
	UPROPERTY(EditAnywhere, Category = "Data")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FVector FakeSpawner = FVector(2900, 2590, 2920);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> SupportItem;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> CatItem;
	
	//TODO:ici

private:
	bool AreAlive = true;
	FTimerHandle TimerHandleNoise;
};
