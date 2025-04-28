// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpectatorComponent.generated.h"


class APrisonerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API USpectatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USpectatorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(Server, Reliable, WithValidation)
	void SwitchCamera(bool Next, APrisonerController* MyController);
	
public:


protected:
	virtual void BeginPlay() override;
	APrisonerController *CamSpectatorPawn = nullptr;
};
