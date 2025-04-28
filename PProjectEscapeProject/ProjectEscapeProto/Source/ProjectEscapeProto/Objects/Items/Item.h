// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectEscapeProto/Objects/InteractableObjects.h"
#include "Item.generated.h"

class APrisoner;

UCLASS()
class PROJECTESCAPEPROTO_API AItem : public AInteractableObjects
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void UseItem();
	virtual void StopUsingItem();
	void SetVisibilityOfObj(bool NeedToBeInvisible);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetVisibility(bool NeedToBeInvisible);
	virtual bool CanInteract_Implementation(AActor* Actor) override;
	virtual bool TryInteract_Implementation(AActor* Actor) override;

	void ConsumeItem();
	UFUNCTION(NetMulticast, Reliable)
	void OnBeginToInteract();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginToInteractBP();
	FVector GetCustomScaleOnHand() const;
	UFUNCTION(BlueprintImplementableEvent)
	void InvalidPrevisuMaterialBP();
	UFUNCTION(BlueprintImplementableEvent)
	void ValidPrevisuMaterialBP();
	UFUNCTION()
	FName GetSocketHandName() const { return SocketHandName; }

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnRep_IsInvisible();
	bool CanBeReused();

public:
	UPROPERTY(EditAnywhere)
	FString ItemName;
	UPROPERTY(EditAnywhere)
	AActor* ObjectOwner;
	UPROPERTY(EditAnywhere)
	bool InventoryItem = true;

	UPROPERTY(ReplicatedUsing = OnRep_IsInvisible)
	bool IsInvisible;
	UPROPERTY(EditAnywhere)
	bool HavePrevisu = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector CustomScaleOnDrop = FVector(1, 1, 1);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector CustomScaleOnHand = FVector(0.5f, 0.5f, 0.5f);
	float TimeToBeReusable;
	float CurrentTimeToBeReusable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* PrevisuTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ItemSprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SocketHandName = "ItemSocket";
};
