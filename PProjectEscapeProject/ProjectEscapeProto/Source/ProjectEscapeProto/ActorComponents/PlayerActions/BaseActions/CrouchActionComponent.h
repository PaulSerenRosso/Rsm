// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Default/PlayerActionStateComponent.h"
#include "CrouchActionComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UCrouchActionComponent : public UPlayerActionStateComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCrouchActionComponent();
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void MyServer_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void MyMulticast_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual bool CanExecuteAction() override;
	virtual EPlayerActionState GetPlayerActionState() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
