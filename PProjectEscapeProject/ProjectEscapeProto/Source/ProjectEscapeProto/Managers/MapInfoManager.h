// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapInfoManager.generated.h"

USTRUCT(Blueprintable)
struct FArrayOfActors
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<AActor*> Actors;
};

UCLASS()
class PROJECTESCAPEPROTO_API AMapInfoManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMapInfoManager();
	virtual void Tick(float DeltaTime) override;
	TArray<AActor*> GetActorWithTag(FString Tag);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FString, FArrayOfActors> AllTaggedActorsBis;

private:

};
