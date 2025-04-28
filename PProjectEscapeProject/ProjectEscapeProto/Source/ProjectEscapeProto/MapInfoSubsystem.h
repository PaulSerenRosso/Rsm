// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapInfoSubsystem.generated.h"

class AMapInfoManager;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UMapInfoSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
	

public:
		TWeakObjectPtr<AMapInfoManager> MapInfoManager;
};
