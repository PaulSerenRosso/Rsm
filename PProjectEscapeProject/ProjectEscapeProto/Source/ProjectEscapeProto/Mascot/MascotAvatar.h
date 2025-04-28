// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MascotAvatar.generated.h"


class UMascotQuotesData;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UMascotAvatar : public UActorComponent
{
	GENERATED_BODY()

public:
	UMascotAvatar();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintImplementableEvent)
	void MascotNeedToTalk(const FString& TextToSay, bool NeedToStayPrinted);
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void PrintLine(const FString& TextToPrint, bool NeedToStayPrinted = false);
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void InitToNewPhasis(const FString& TextToPrint, const float PhaseTimer, const FString& QuestDescription, bool NeedToStayPrinted = false);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_InitToNewPhasis(const FString& TextToPrint, const float PhaseTimer, const FString& QuestDescription, bool NeedToStayPrinted = false);
	UFUNCTION(BlueprintCallable)
	void PrintLineAlone(const FString& TextToPrint, bool NeedToStayPrinted = false);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMascotQuotesData* MascotQuotesData;
};
