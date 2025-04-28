// Fill out your copyright notice in the Description page of Project Settings.


#include "Balloon.h"

ABalloon::ABalloon()
{
	bReplicates = true;
}

void ABalloon::ValidateBalloon()
{
	OnBalloonFinished.Broadcast(true);
}

void ABalloon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
