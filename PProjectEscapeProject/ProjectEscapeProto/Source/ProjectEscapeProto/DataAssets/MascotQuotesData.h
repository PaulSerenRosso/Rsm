// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MascotQuotesData.generated.h"

UCLASS()
class PROJECTESCAPEPROTO_API UMascotQuotesData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText PrintQuestQuote;
	
};
