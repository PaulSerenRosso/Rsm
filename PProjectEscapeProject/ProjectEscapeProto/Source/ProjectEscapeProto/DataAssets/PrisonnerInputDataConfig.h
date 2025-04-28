// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PrisonnerInputDataConfig.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UPrisonnerInputDataConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* WalkAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* CrouchAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SprintAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InteractAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* DropAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* MenuOptionsAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* UseItemAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SwapItemAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SwitchPrevCamAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* SwitchNextCamAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* PushAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* PrintQuestAction;
};
