// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hitable.generated.h"


UINTERFACE(MinimalAPI)
class UIHitable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTESCAPEPROTO_API IIHitable
{
	GENERATED_BODY()

public:
	
	virtual bool CanHit(AActor* Actor) = 0;
	virtual bool TryHit(AActor* Actor) = 0;
};