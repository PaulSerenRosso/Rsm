// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Default/PlayerActionComponent.h"
#include "JumpActionComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UJumpActionComponent : public UPlayerActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UJumpActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
