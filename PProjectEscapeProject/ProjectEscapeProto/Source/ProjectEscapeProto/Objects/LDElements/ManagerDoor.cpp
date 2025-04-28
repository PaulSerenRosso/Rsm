// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerDoor.h"


// Sets default values
AManagerDoor::AManagerDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DoorPoint = CreateDefaultSubobject<USceneComponent>("DoorPoint");
	ExitInteractPoint = CreateDefaultSubobject<USceneComponent>("ExitInteractPoint");
	EnterInteractPoint = CreateDefaultSubobject<USceneComponent>("EnterInteractPoint");
	ExitInteractPoint->SetupAttachment(DoorPoint);
	DoorPoint->SetupAttachment(EnterInteractPoint);
	RootComponent = EnterInteractPoint;
}

// Called when the game starts or when spawned
void AManagerDoor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AManagerDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

