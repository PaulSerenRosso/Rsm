// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "MascotMetricsAnalyzer.generated.h"

class AMascot;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTESCAPEPROTO_API UMascotMetricsAnalyzer : public UActorComponent
{
	GENERATED_BODY()

	public :
	void Init(AMascot* Mascot);
	UFUNCTION(BlueprintImplementableEvent)
	EMascotTag MascotTagAnalyseMetrics();
	virtual EMascotTag MascotTagAnalyseMetricsCPP();

protected:
	UPROPERTY(BlueprintReadOnly)
	AMascot* MyMascot;
};
