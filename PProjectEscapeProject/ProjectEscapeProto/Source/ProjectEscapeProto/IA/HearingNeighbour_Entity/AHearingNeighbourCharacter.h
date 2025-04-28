// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/IA/AICharacter.h"
#include "ProjectEscapeProto/Objects/LDElements/ManagerDoor.h"
#include "AHearingNeighbourCharacter.generated.h"

class APrisoner;
class UJeremiePhase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertDelegate, APrisoner*, Target);

UCLASS()
class PROJECTESCAPEPROTO_API AAHearingNeighbourCharacter : public AAICharacter
{
	GENERATED_BODY()

public:
	AAHearingNeighbourCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnAlert(APrisoner* Target);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnAlert(APrisoner* Target);
	UFUNCTION(BlueprintImplementableEvent)
	void OnLostSighPlayer(APrisoner* Target);
	UFUNCTION(BlueprintImplementableEvent)
	void OnEndAlert(APrisoner* Target);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnInteractWithManagerDoor(AManagerDoor* ManagerDoor, bool IsExit);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnAlertDelegate OnDetectPlayer;
	
protected:
	UPROPERTY(EditAnywhere)
	UAudioComponent* WalkAudioComponent;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UJeremiePhase>> Phases;
	float ElapsedTime = 0.0f;

};
