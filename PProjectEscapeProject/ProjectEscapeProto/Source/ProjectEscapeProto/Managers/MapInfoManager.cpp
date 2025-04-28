// Fill out your copyright notice in the Description page of Project Settings.


#include "MapInfoManager.h"

#include "ProjectEscapeProto/MapInfoSubsystem.h"
#include "ProjectEscapeProto/OnlineGameInstance.h"


// Sets default values
AMapInfoManager::AMapInfoManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMapInfoManager::BeginPlay()
{
	Super::BeginPlay();
	Cast<UOnlineGameInstance>(GetGameInstance())->MapInfoSubsystem->MapInfoManager = this;
}

void AMapInfoManager::Destroyed()
{
	Super::Destroyed();
	UOnlineGameInstance* GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (GameInstance->MapInfoSubsystem)
		{
			GameInstance->MapInfoSubsystem->MapInfoManager = nullptr;
		}
	}
}


void AMapInfoManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// AMapInfoManager* AMapInfoManager::GetInstance()
// {
// 	return Instance;
// }

TArray<AActor*> AMapInfoManager::GetActorWithTag(FString Tag)
{
	TArray<AActor*> Actors;

	if (AllTaggedActorsBis.Num() == 0) return Actors;
	if (AllTaggedActorsBis.Contains(Tag))
	{
		FArrayOfActors ArrayOfActors = AllTaggedActorsBis[Tag];
		if (ArrayOfActors.Actors.Num() > 0)
		{
			Actors = ArrayOfActors.Actors;
		}
	}
	return Actors;
}
