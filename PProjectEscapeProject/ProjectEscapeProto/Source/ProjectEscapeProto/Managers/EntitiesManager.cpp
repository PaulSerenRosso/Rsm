// Fill out your copyright notice in the Description page of Project Settings.


#include "EntitiesManager.h"

#include "ActionTimerManager.h"
#include "MapInfoManager.h"
#include "ProjectEscapeProto/OnlineGameInstance.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/DataAssets/SpawnEntityData.h"
#include "ProjectEscapeProto/IA/AICharacter.h"

// Sets default values
AEntitiesManager::AEntitiesManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEntitiesManager::BeginPlay()
{
	Super::BeginPlay();
	// if (HasAuthority())
	// {
	// 	AOnlineGameMode* OnlineGameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	//
	// 	OnlineGameMode->OnStartGame.AddDynamic(this, &AEntitiesManager::InitSpawnEntity);
	// }
}

void AEntitiesManager::SpawnEntity(int ID)
{
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN ENTITY : START SPAWN ENTITY"));
	TArray<AActor*> Spawners = Cast<UOnlineGameInstance>(GetWorld()->GetGameInstance())->MapInfoSubsystem->
		MapInfoManager->GetActorWithTag(CurrentEntityToSpawn[ID]->TagLocationOfSpawning);
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN ENTITY : SPAWNERS NUM : %d"), Spawners.Num());
	int Index = FMath::RandRange(0, Spawners.Num() - 1);
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN ENTITY : INDEX : %d"), Index);
	FTransform SpawnTransform = Spawners[Index]->GetActorTransform();
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN ENTITY : SPAWN TRANSFORM : %s"), *SpawnTransform.ToString());
	AAICharacter* CharacterSpawned = Cast<AAICharacter>(GetWorld()->SpawnActor(CurrentEntityToSpawn[ID]->EntityToSpawn, &SpawnTransform));
	CharacterSpawned->Init(CurrentEntityToSpawn[ID].Get());
	CurrentEntity.Add(CharacterSpawned);
	;
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN ENTITY : ENTITY NORMALLY SPAWNED"));
	/*if (CurrentEntity.IsValid())
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Entity spawned"));
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Entity not spawned"));*/
}

void AEntitiesManager::DespawnEntity(AAICharacter* EntityToDespawn)
{
	if(EntityToDespawn != nullptr)
	{
		if(CurrentEntity.Contains(EntityToDespawn))
		{
			CurrentEntity.Remove(EntityToDespawn);
			EntityToDespawn->Destroy();
		}
	}
}

void AEntitiesManager::InitSpawnEntity(TArray<USpawnEntityData*> EntitiesToSpawn)
{
	for (auto Entity : EntitiesToSpawn)
	{
		if (!Entity) continue;
		CurrentEntityToSpawn.Add(Entity);
		CurrentTimers.Add(Entity->TimerBeforeSpawnEntity);
	}
	
	// FActionTimer* FirstTicker = AActionTimerManager::AddActionTimer(CurrentEntityToSpawn->TimerBeforeSpawnEntity, -1);
	// FirstTicker->OnActionTimerManagerTick.AddDynamic(this, &AEntitiesManager::SpawnEntity);
	//UE_LOG(LogTemp, Warning, TEXT("SPAWN ENTITY : CURRENT TIMER : %f"), CurrentTimer);
}

// Called every frame
void AEntitiesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < CurrentTimers.Num(); i++)
	{
		if (CurrentTimers[i] > 0.0f)
		{
			CurrentTimers[i] -= DeltaTime;
			//UE_LOG(LogTemp, Warning, TEXT("SPAWN ENTITY : CURRENT TIMER : %f"), CurrentTimer);
			if (CurrentTimers[i] <= 0.0f)
			{
				//UE_LOG(LogTemp, Warning, TEXT("SPAWN ENTITY : TRY TO SPAWN ENTITY AT END OF TIMER"));
				SpawnEntity(i);
				i--;
			}
		}
	}
}

void AEntitiesManager::DestroyAllEntities() const
{
	for (auto Entity : CurrentEntity)
	{
		if (Entity.IsValid())
		{
			Entity->Destroy();
		}
	}
}
