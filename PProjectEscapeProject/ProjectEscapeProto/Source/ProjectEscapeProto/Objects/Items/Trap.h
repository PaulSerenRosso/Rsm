// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "ProjectEscapeProto/Interfaces/Grabable.h"
#include "Trap.generated.h"

class UTweakValueTrapData;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivateTrapDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIncapacitedTrapOnActivate);

UCLASS()
class PROJECTESCAPEPROTO_API ATrap : public AItem, public IIGrabable
{
	GENERATED_BODY()

public:
	ATrap();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_ArmTrap();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_ArmTrap();
	virtual void ArmTrap();
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_DisarmTrap();
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_DisarmTrap();
	virtual void ActivateTrap(AActor* OtherActor);
	bool GetIsArmed() const { return IsArmed; }
	virtual void UseItem() override;
	virtual bool CanInteract_Implementation(AActor* Actor) override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult);
	UFUNCTION(BlueprintImplementableEvent, Category = "IncapacitedTrap")
	void BP_ArmTrap();
	UFUNCTION(BlueprintImplementableEvent, Category = "IncapacitedTrap")
	void BP_ActivateTrap(AActor* StunnedActor);
	UFUNCTION(BlueprintImplementableEvent, Category = "IncapacitedTrap")
	void BP_UseItem();

public:
	UPROPERTY(BlueprintAssignable)
	FOnActivateTrapDelegate OnActivateTrap;
	FIncapacitedTrapOnActivate OnTrapArmed;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap")
	UBoxComponent* BoxComponent;
	UPROPERTY(Replicated)
	bool IsArmed = false;
	float MaxDistanceActivateTrap = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IncapacitedTrap")
	UTweakValueTrapData* TweakeableData;
};
