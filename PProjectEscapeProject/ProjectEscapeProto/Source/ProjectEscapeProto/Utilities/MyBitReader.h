// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MyBitReader.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UMyBitReader : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(const TArray<uint8>& InData);
	UFUNCTION(BlueprintCallable)
	FString GetNextStringValue();
	UFUNCTION(BlueprintCallable)
	float GetNextFloatValue();
	UFUNCTION(BlueprintCallable)
	int GetNextIntValue();
	UFUNCTION(BlueprintCallable)
	bool GetNextBoolValue();
	UFUNCTION(BlueprintCallable)
	FVector2D GetNextVector2DValue();
	uint64 GetNextUInt64Value();

protected:
 void	IncreaseOffset(int Amount);
protected:

	TArray<uint8> Data;
	int Offset;


public:
	

};
