// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTemplate.h"
#include "MyTemplateVector2D.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UMyTemplateVector2D : public UMyTemplate
{
	GENERATED_BODY()
public:
	FVector2D Vector2DValue;
};
