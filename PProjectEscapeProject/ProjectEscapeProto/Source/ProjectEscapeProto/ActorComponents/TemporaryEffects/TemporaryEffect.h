// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TemporaryEffect.generated.h"


class UPushEffectComponent;
class USlowEffectComponent;
class UStunEffectComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
	class PROJECTESCAPEPROTO_API UTemporaryEffect : public USceneComponent
{
	GENERATED_BODY()

public:
	UTemporaryEffect(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable)
	void StunCharacter(float StunTime);
	UFUNCTION(BlueprintCallable)
	void PushCharacter(float PushTime, FVector Direction, float Force);
	UFUNCTION(BlueprintCallable)
	void SlowCharacter(float Ratio, float Duration);
	UFUNCTION(BlueprintCallable)
	void RemoveStun();
	UFUNCTION(BlueprintCallable)
	void RemoveSlow();
	bool IsAlreadyPushed() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	TWeakObjectPtr<UStunEffectComponent> StunEffectComponent = nullptr;
	TWeakObjectPtr<USlowEffectComponent> SlowEffectComponent = nullptr;
	TWeakObjectPtr<UPushEffectComponent> PushEffectComponent = nullptr;
		
};
