// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaSystem.generated.h"

class APrisoner;
class UTweakeableValueCharacterData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterConsumeStaminaDelegate);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UStaminaSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStaminaSystem();
	UFUNCTION(Server, Reliable)
	void Server_ResetStamina();
	UFUNCTION(Server, Reliable)
	void Server_InitStaminaSystem();
	UFUNCTION(Server, Reliable)
	void Multicast_InitStaminaSystem();
	UFUNCTION(Server, Unreliable)
	void Server_ConsumeStamina(float value);
	UFUNCTION(BlueprintPure)
	float GetCurrentStamina();
	UFUNCTION(BlueprintPure)
	float GetCurrentStaminaMax();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnRep_CurrentStaminaAmount() const;
	UFUNCTION()
	void OnRep_CurrentStaminaMax() const;

	void SetCurrentStamina(float NewValue);
	void SetCurrentStaminaMax(float NewMaxStaminaValue);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	

public:
	UPROPERTY(BlueprintAssignable)
	FCharacterConsumeStaminaDelegate OnUpdateStamina;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UTweakeableValueCharacterData* TweakeableData;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_CurrentStaminaAmount)
	float CurrentStaminaAmount = 0.0f;
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_CurrentStaminaMax)
	float CurrentStaminaMax = 0.0f;
	bool CanRecoverStamina = false;
	float StaminaDelayBeforeRecoveryTimer = 0.0f;
	APrisoner* MyPrisoner;
	bool IsConsumeStamina= false;

};
