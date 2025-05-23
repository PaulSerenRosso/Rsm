// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class PROJECTESCAPEPROTO_API IIInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInteract(AActor* Actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryInteract(AActor* Actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryStopInteract(AActor* Actor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetInteractionName();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetTypeInteractionName();
	// Ajouts pour les slots
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) //VERIFIE LA DISTANCE
	bool IsInteractionValid(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) //EN CAS DE MORT OU JSP
	void OnInteractionInterrupted(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) //POUR PRESS E "X" SECONDE
	float GetInteractionHoldTime();
};
