// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/DataAssets/SpawnEntityData.h"


// Sets default values
AEntityController::AEntityController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEntityController::BeginPlay()
{
	Super::BeginPlay();

	
}

void AEntityController::OnPreviousPhasisEnded(int Index)
{
	GetBlackboardComponent()->SetValueAsBool("IsEndPhasis", true);
}


// Called every frame
void AEntityController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEntityController::Init(USpawnEntityData* SpawnEntityData)
{
	if(SpawnEntityData)
	{
		if(UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent))
		{
			GetBlackboardComponent()->SetValueAsObject("SpawnEntityData",SpawnEntityData);
			AOnlineGameMode* OnlineGameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
			if(OnlineGameMode)
			{
				OnlineGameMode->PreviousPhasisEnded.AddDynamic(this, &AEntityController::OnPreviousPhasisEnded);
			}
		}
	}
	RunBehaviorTree(BehaviorTree);


}



