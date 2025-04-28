// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Default/PlayerActionStateComponent.h"
#include "SprintActionComponent.generated.h"

class UStaminaSystem;
class UTweakeableValueCharacterData;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API USprintActionComponent : public UPlayerActionStateComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USprintActionComponent();
	virtual void InitMulticastAction(APrisoner* InPrisoner) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual EPlayerActionState GetPlayerActionState() override;
	virtual void InitServerAction(APrisoner* InPrisoner) override;
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void MyServer_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual bool CanExecuteAction() override;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UTweakeableValueCharacterData* TweakeableData;
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	UStaminaSystem* StaminaSystem;
};
