// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManagerDoor.generated.h"

UCLASS()
class PROJECTESCAPEPROTO_API AManagerDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AManagerDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* ExitInteractPoint;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* EnterInteractPoint;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* DoorPoint;
};
