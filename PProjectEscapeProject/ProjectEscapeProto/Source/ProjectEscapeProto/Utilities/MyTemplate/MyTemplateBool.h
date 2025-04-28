// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTemplate.h"
#include "MyTemplateBool.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UMyTemplateBool : public UMyTemplate
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	bool BoolValue;
};
