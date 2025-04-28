// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "IncapacitedTrap.generated.h"

class APrisoner;
class UTweakValueTrapData;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API AIncapacitedTrap : public ATrap
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void ArmTrap() override;
	virtual void Multicast_DisarmTrap() override;
	
	virtual void ActivateTrap(AActor* OtherActor) override;
	virtual void UseItem() override;
	
protected:

	//APrisoner* PlayerToIncapacited;
	float CurrentStunDuration;
	float CurrentSlowRatio;
};
