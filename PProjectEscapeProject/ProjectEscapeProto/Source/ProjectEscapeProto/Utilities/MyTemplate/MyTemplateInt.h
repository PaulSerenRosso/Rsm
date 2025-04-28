// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTemplate.h"
#include "MyTemplateInt.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UMyTemplateInt : public UMyTemplate
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int IntValue;
};
