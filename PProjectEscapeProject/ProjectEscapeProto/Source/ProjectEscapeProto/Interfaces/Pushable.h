// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Pushable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPushable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class PROJECTESCAPEPROTO_API IPushable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanBePushed(AActor* Actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryPush(float Duration, FVector Direction, float Force);
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	// bool TryStopInteract(AActor* Actor);
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	// FString GetInteractionName();
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	// FString GetTypeInteractionName();
};
