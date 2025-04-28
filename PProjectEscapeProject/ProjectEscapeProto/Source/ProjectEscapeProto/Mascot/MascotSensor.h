// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "MascotSensor.generated.h"

class APrisonerController;
class AMascot;

UCLASS(Blueprintable, BlueprintType)
class PROJECTESCAPEPROTO_API UMascotSensor : public UActorComponent
{
	GENERATED_BODY()

public:
	// UFUNCTION(Server, Reliable)
	virtual  void InitSensor(UWorld* WorldContext, AMascot* Mascot, APrisonerController * PC);

protected:
	TArray<AActor*> GetSpecificActorsOnWorld(UWorld* WorldContext, TSubclassOf<AActor> ActorClass);
	virtual void UpdateMetrics();

protected:
	UPROPERTY(BlueprintReadOnly)
	UWorld* MyWordContext;
	UPROPERTY(BlueprintReadOnly)
	AMascot* MyMascot;
	TWeakObjectPtr<APrisonerController> MyPC;
};
