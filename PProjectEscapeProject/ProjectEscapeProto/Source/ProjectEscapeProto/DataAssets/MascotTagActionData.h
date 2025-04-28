// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "MascotTagActionData.generated.h"

class UMascotTagAction;
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTESCAPEPROTO_API UMascotTagActionData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<UMascotTagAction> TagActionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EMascotTag>> InputMascotTag;
};
