// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBornPathFinder.h"

// Sets default values
AAIBornPathFinder::AAIBornPathFinder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AAIBornPathFinder::SortArrayByCost(TArray<AAIBornPathFinder*>& Array)
{
	//bool bIsSorted = false;
	for (int i = 0; i < Array.Num(); i++)
	{
		for (int j = i + 1; j < Array.Num(); j++)
		{
			if (Array[i]->GetCost() > Array[j]->GetCost())
			{
				AAIBornPathFinder* Temp = Array[i];
				Array[i] = Array[j];
				Array[j] = Temp;
				//bIsSorted = true;
				j--;
			}
		}
	}
}

// Called when the game starts or when spawned
void AAIBornPathFinder::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAIBornPathFinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
