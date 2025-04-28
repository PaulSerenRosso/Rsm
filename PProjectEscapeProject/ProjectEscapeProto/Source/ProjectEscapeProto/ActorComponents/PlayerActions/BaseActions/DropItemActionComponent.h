// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionComponent.h"
#include "DropItemActionComponent.generated.h"


class UInventorySystem;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UDropItemActionComponent : public UPlayerActionComponent
{
	GENERATED_BODY()

public:
	UDropItemActionComponent();

protected:
	virtual void InitServerAction(APrisoner* InPrisoner) override;
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual bool CanExecuteAction() override;

protected:
	TWeakObjectPtr<UInventorySystem> InventorySystem;
};
