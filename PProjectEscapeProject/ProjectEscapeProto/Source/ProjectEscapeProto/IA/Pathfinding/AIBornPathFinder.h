// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIBornPathFinder.generated.h"

UCLASS()
class PROJECTESCAPEPROTO_API AAIBornPathFinder : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIBornPathFinder();
	virtual void Tick(float DeltaTime) override;
	void IncreaseCost() { Cost++; }
	int GetCost() const { return Cost; }
	static void SortArrayByCost(TArray<AAIBornPathFinder*>& Array);

protected:
	virtual void BeginPlay() override;


protected:
	int Cost = 0;
	UStaticMeshComponent* Mesh;
};
