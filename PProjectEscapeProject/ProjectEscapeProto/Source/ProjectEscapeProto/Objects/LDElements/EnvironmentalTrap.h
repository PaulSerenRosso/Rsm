// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectEscapeProto/Interfaces/IInteractable.h"
#include "EnvironmentalTrap.generated.h"

class UTweakValueTrapData;

UCLASS()
class PROJECTESCAPEPROTO_API AEnvironmentalTrap : public AActor, public IIInteractable
{
	GENERATED_BODY()
	
public:	
	AEnvironmentalTrap();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool CanInteract_Implementation(AActor* Actor) override;
	virtual bool TryInteract_Implementation(AActor* Actor) override;
	UFUNCTION(BlueprintImplementableEvent)
	void ActivateEnvironmentalTrapBP();
	
	UFUNCTION(BlueprintCallable)
	void StunCharacters(TArray<AActor*> Actors);
	UFUNCTION(BlueprintCallable)
	void SlowCharacters(TArray<AActor*> Actors);
	UFUNCTION(BlueprintCallable)
	void BlindCharacters(TArray<AActor*> Actors);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UTweakValueTrapData *TrapData;
	UPROPERTY(Replicated)
	bool IsCurrentlyActivated = false;
	

};
