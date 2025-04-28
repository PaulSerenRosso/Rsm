// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "BinaryDataSubsystem.generated.h"

class UMyBitReader;
class UMyBitWriter;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UBinaryDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
public:
	UPROPERTY(BlueprintReadWrite)
	UMyBitWriter* BitWriter;
	UPROPERTY(BlueprintReadWrite)
	UMyBitReader* BitReader;
	void SetNetDriver(UNetDriver* NetDriver);

};
