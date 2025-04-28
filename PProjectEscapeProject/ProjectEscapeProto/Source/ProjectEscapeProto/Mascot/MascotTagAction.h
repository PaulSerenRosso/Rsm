// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MascotTagAction.generated.h"

enum EMascotTag : uint8;
class APrisonerController;
class UMascotAvatar;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTESCAPEPROTO_API UMascotTagAction : public UActorComponent
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintNativeEvent)
	void Execute();

public:
	UPROPERTY(BlueprintReadOnly)
	APrisonerController* PC;
	UPROPERTY(EditAnywhere)
	bool ConsumeTagAction = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EMascotTag>> InputMascotTag;
};
