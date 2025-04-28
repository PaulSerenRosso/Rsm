// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MascotNode.generated.h"

class UMascotDependencyProvider;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UMascotNode : public UObject
{
	GENERATED_BODY()

	TWeakObjectPtr<UMascotDependencyProvider> DependencyProvider;
};
