// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomPathFindingManager.generated.h"

UCLASS()
class PROJECTESCAPEPROTO_API ARoomPathFindingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomPathFindingManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
