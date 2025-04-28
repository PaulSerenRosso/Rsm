// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionComponent.h"
#include "SwitchItemActionComponent.generated.h"


class UInventorySystem;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API USwitchItemActionComponent : public UPlayerActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USwitchItemActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void InitMulticastAction(APrisoner* InPrisoner) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
protected:
	TWeakObjectPtr<UInventorySystem> InventorySystem;
};
