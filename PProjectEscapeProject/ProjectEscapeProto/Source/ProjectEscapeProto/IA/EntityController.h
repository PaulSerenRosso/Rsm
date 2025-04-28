// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "AIController.h"
#include "EntityController.generated.h"

class USpawnEntityData;

UCLASS()
class PROJECTESCAPEPROTO_API AEntityController : public AAIController
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init(USpawnEntityData* SpawnEntityData);
	UBlackboardComponent* BlackboardComponent;

protected:
	AEntityController();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnPreviousPhasisEnded(int Index);
public:
	// Sets default values for this actor's properties
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;



		
};
