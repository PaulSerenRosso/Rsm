// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Default/PlayerActionComponent.h"
#include "MoveActionComponent.generated.h"


class UMyTemplateFloat;
class UTweakeableValueCharacterData;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UMoveActionComponent : public UPlayerActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMoveActionComponent();
	
	float GetSlowRatio() const { return CurrentSlowRatio; }
	void SetSlowRatio(float NewRatio) { CurrentSlowRatio = NewRatio; }
	
	virtual void InitMulticastAction(APrisoner* InPrisoner) override;
	virtual void InitServerAction(APrisoner* InPrisoner) override;
	virtual void AskExecuteAction(const TArray<uint8>& Payload = TArray<uint8>(), const TArray<AActor*>& Actors = TArray<AActor*>()) override;
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload = TArray<uint8>(), const TArray<AActor*>& Actors= TArray<AActor*>()) override;
	virtual void MyMulticast_ExecuteAction(const TArray<uint8>& Payload = TArray<uint8>(), const TArray<AActor*>& Actors= TArray<AActor*>()) override;
	
	
	void AssignMaxWalkSpeed(float Value);
	void AssignMaxCrouchWalkSpeed(float Value);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	


public:

	TWeakObjectPtr<UTweakeableValueCharacterData> TweakableData;
	int32* PlayerActionStateMask;
private:
	UPROPERTY(Replicated)
	float CurrentSlowRatio = 1.0f;
	float LocalWalkSpeed;
	float LocalCrouchWalkSpeed;
};
