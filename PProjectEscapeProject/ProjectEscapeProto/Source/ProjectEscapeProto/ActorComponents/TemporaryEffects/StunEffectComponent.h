// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StunEffectComponent.generated.h"


class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPEPROTO_API UStunEffectComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStunEffectComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void StunCharacter(float StunTime);
	void RemoveStun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:	
	TArray<float> StunTimer;
	TWeakObjectPtr<UCharacterMovementComponent> AIMovement = nullptr;
		
};
