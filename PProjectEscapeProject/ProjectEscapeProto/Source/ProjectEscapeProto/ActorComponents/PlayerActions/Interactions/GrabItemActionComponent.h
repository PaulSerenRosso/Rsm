// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionTimedComponent.h"
#include "GrabItemActionComponent.generated.h"

class AItem;
class UInventorySystem;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UGrabItemActionComponent : public UPlayerActionTimedComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabItemActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Server_ExecuteAction_Implementation(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void InitServerAction(APrisoner* InPrisoner) override;
	virtual float GetCurrentTime() override;
	virtual void EndTimerAction() override;
	virtual void Server_StopAction_Implementation(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual EPlayerActionState GetPlayerActionState() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UInventorySystem *InventorySystem;

protected:
	AItem *GrabItem; 
};
