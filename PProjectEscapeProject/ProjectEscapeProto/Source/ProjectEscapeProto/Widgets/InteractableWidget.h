// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UInteractableWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeTextInteract(const FString& TextToPrint);
};
