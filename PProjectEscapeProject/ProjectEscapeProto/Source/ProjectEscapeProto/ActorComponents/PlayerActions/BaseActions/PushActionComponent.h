// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionComponent.h"
#include "PushActionComponent.generated.h"

class UStaminaSystem;
class UCameraComponent;
class UPushData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHaveRessourceForPushChangedDelegate);
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UPushActionComponent : public UPlayerActionComponent
{
	GENERATED_BODY()

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void InitServerAction(APrisoner* InPrisoner) override;
	virtual bool CanExecuteAction() override;
	UFUNCTION()
	void OnUpdatedStamina();
	UFUNCTION()
	void OnRep_HaveResourceForPush();
	UFUNCTION(BlueprintPure)
	bool GetHaveResourceForPush();
public:
	UPROPERTY(BlueprintAssignable)
	FHaveRessourceForPushChangedDelegate OnHaveResourceForPushChanged;
	
protected:
	UPROPERTY( ReplicatedUsing=OnRep_HaveResourceForPush)
	bool HaveResourceForPush;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UPushData* PushData = nullptr;
	float CurrentCooldown = 0.f;
	UPROPERTY( BlueprintReadOnly, Category = "Data")
	UStaminaSystem* StaminaSystem;
	bool CanConsumeMinimumStaminaCost = true;
	void UpdateHaveResourceForPush();
	TWeakObjectPtr<UCameraComponent> CameraComponent;
};
