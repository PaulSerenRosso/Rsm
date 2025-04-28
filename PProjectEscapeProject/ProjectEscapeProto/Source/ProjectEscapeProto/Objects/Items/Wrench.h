// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ProjectEscapeProto/Interfaces/Grabable.h"
#include "Wrench.generated.h"

class UTweakValueWrenchData;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API AWrench : public AItem, public IIGrabable
{
	GENERATED_BODY()

	public:
		AWrench();
		virtual void UseItem() override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wrench")
	UTweakValueWrenchData* WrenchData;
	int Durability = 5;
};
