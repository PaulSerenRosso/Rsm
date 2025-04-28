// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SlowEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPEPROTO_API USlowEffectComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlowEffectComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SlowCharacter(float Ratio, float Duration);
	void RemoveSlow();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	float CurrentSlowRatio = 1.0f;
	float MinimumSlowRatio = 1.0f;
	float SlowTimer = 0.0f;
	float MaxSlowTimer = 0.0f;
		
};
