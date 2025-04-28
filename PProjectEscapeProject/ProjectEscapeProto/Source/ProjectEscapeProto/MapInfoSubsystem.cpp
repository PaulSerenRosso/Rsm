// Fill out your copyright notice in the Description page of Project Settings.


#include "MapInfoSubsystem.h"

void UMapInfoSubsystem::Deinitialize()
{
	Super::Deinitialize();
	MapInfoManager = nullptr;
}
