// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MascotDependencyProvider.generated.h"

class UMyTemplate;
enum EMascotKnowledgeType : uint8;
class APrisoner;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UMascotDependencyProvider : public UObject
{
	GENERATED_BODY()

protected:
	TMap<EMascotKnowledgeType, UMyTemplate*> Metrics;
	TWeakObjectPtr<APlayerController> PlayerController;
	TWeakObjectPtr<APrisoner> Prisoner;
};
