// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/Mascot/MascotMetricsAnalyzer.h"
#include "AnalyzerLastEnterElevatorIsMe.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UAnalyzerLastEnterElevatorIsMe : public UMascotMetricsAnalyzer
{
	GENERATED_BODY()

	virtual EMascotTag MascotTagAnalyseMetricsCPP() override;
	
};
