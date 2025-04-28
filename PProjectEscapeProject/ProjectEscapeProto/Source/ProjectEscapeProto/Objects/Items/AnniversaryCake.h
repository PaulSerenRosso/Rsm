// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AnniversaryCake.generated.h"

class UCharacterMovementComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCakeArrivedDelegate);

class UAnniversaryCakeData;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API AAnniversaryCake : public AItem
{
	GENERATED_BODY()

public:
	AAnniversaryCake();
	void CakeArrived();
	
protected:
	void MoveToPos(FVector NewLocation);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MoveToPos(FVector NewLocation);

	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual bool TryInteract_Implementation(AActor* Actor) override;
	virtual bool TryStopInteract_Implementation(AActor* Actor) override;
	void TryToMoveCake(float DeltaSeconds);
	UFUNCTION()
	void OnFirstPlayerDied(int32 PlayerId);
	UFUNCTION()
	void OnSecondPlayerDied(int32 PlayerId);

public:
	FCakeArrivedDelegate OnCakeArrived;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnniversaryCake")
	UAnniversaryCakeData* CakeData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnniversaryCake")
	USceneComponent* FirstPersonPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnniversaryCake")
	USceneComponent*SecondPersonPosition;
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnniversaryCake")
	// UCharacterMovementComponent* CakeMovement;

	TWeakObjectPtr<APrisoner> FirstPlayer = nullptr;
	TWeakObjectPtr<APrisoner> SecondPlayer = nullptr;
};
