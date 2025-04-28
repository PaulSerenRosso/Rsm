// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntitiesManager.generated.h"

class AAICharacter;
class USpawnEntityData;

UCLASS()
class PROJECTESCAPEPROTO_API AEntitiesManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEntitiesManager();
	virtual void Tick(float DeltaTime) override;
	void DestroyAllEntities() const;
	void InitSpawnEntity(TArray<USpawnEntityData*> EntitiesToSpawn);
	UFUNCTION()
	void DespawnEntity(AAICharacter* EntityToDespawn);

protected:
	UFUNCTION()
	void SpawnEntity(int ID);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	TArray<TWeakObjectPtr<USpawnEntityData>> CurrentEntityToSpawn;
	TArray<TWeakObjectPtr<AActor>> CurrentEntity;

	TArray<float> CurrentTimers;
};
