// Fill out your copyright notice in the Description page of Project Settings.


#include "MascotMetricsAnalyzer.h"

void UMascotMetricsAnalyzer::Init(AMascot* Mascot)
{
	MyMascot = Mascot;
}

EMascotTag UMascotMetricsAnalyzer::MascotTagAnalyseMetricsCPP()
{
	return EMascotTag::None;
}
