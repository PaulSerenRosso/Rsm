// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalyzerElevatorClosed.h"

#include "ProjectEscapeProto/Mascot/Mascot.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplateBool.h"

EMascotTag UAnalyzerElevatorClosed::MascotTagAnalyseMetricsCPP()
{
	UMyTemplateBool* IsClosed = Cast<UMyTemplateBool>(MyMascot->GetSpecificKnowledge(ElevatorClosed));
	if (IsClosed && IsClosed->BoolValue)
	{
		return EMascotTag::IsPrisonerFailToEnterOnElevator;
	}
	return EMascotTag::None;
}
