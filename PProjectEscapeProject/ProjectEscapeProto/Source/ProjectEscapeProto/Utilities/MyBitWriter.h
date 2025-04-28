// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MyBitWriter.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTESCAPEPROTO_API UMyBitWriter : public UObject
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
	TArray<uint8> GetData();
	UFUNCTION(BlueprintCallable)
	void SetString(FString src);
	UFUNCTION(BlueprintCallable)
	void SetBool(bool src);
	UFUNCTION(BlueprintCallable)
	void SetFloat(float src);
	UFUNCTION(BlueprintCallable)
	void SetInt(int src);
	
	void SetUInt64(uint64 src);
	UFUNCTION(BlueprintCallable)
	void SetVector2D(FVector2D src);

protected:
	TArray<uint8> Data = TArray<uint8>();

	

	
};
