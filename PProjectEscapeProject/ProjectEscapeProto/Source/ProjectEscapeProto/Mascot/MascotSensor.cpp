// Fill out your copyright notice in the Description page of Project Settings.


#include "MascotSensor.h"

#include "EngineUtils.h"
#include "ProjectEscapeProto/Characters/PrisonerController.h"

void UMascotSensor::InitSensor(UWorld* WorldContext, AMascot* Mascot, APrisonerController * PC)
{
	MyWordContext = WorldContext;
	MyMascot = Mascot;
	MyPC = PC;
}

TArray<AActor*> UMascotSensor::GetSpecificActorsOnWorld(UWorld* WorldContext, TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> Actors;
	for (TActorIterator<AActor> ActorItr(WorldContext); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsA(ActorClass))
		{
			Actors.Add(*ActorItr);
		}
	}
	return Actors;
}

void UMascotSensor::UpdateMetrics()
{
	// Do something
}
