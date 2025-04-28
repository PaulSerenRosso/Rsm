// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnActor.generated.h"

UCLASS()
class PROJECTESCAPEPROTO_API ASpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnActor();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Client, Reliable)
	void ActivateWallhack();
	virtual void DeactivateInteraction();
	void SetInteractable(bool Value) { IncompleteInteraction = Value; }
	void SetUsable(bool Value) { CanBeUsed = Value; }
	bool GetInteractable() const { return IncompleteInteraction; }

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintImplementableEvent)
	void BP_DeactivateInteraction();

public:

	UPROPERTY(EditAnywhere)
	bool IsCountHasAnObjective = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StillInteractable = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool IncompleteInteraction = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool CanBeUsed = true;
};
