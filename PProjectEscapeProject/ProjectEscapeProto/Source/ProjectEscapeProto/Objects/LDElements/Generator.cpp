// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "NavAreas/NavArea_Null.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AISense_Hearing.h"
#include "ProjectEscapeProto/DataAssets/GeneratorData.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"
#include "ProjectEscapeProto/Utilities/StructUtilities.h"

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GeneratorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("GeneratorWidget"));
	GeneratorWidget->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	GeneratorWidget->SetRelativeLocation(FVector(0, 0, 100));
	GeneratorWidget->SetWidgetSpace(EWidgetSpace::Screen);
	GeneratorWidget->SetDrawSize(FVector2D(200, 70));
	CurrentStep = 0;

	

}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	GeneratorWidget->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	GeneratorWidget->SetRelativeLocation(FVector(0, 0, 200));
}

void AGenerator::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGenerator, CurrentStep);
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGenerator::CanHit(AActor* Actor)
{
	return CurrentStep < GeneratorData->MaxStepsToRepare;
}

bool AGenerator::TryHit(AActor* Actor)
{
	if (CanHit(this))
	{
		CurrentStep++;
		OnTryToRepared.Broadcast(Actor);
		UFunctionsUtilities::Server_MakeNoise(this, this, GetActorLocation(),
			GeneratorData->LoudnessNoiseEmitted);
		//UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), GeneratorData->LoudnessNoiseEmitted / MAX_NOISE, this, GeneratorData->MaxRangeNoiseEmittec, NAME_None);
		if (CurrentStep >= GeneratorData->MaxStepsToRepare)
		{
			OnRepared.Broadcast(Actor);
			IncompleteInteraction = false;
		}
		return true;
	}
	return false;
}

