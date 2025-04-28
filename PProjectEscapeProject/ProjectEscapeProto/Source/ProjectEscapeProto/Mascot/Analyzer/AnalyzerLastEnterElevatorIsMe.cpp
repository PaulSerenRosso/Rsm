// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalyzerLastEnterElevatorIsMe.h"

#include "ProjectEscapeProto/Characters/PrisonerController.h"
#include "ProjectEscapeProto/Mascot/Mascot.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplateInt.h"

EMascotTag UAnalyzerLastEnterElevatorIsMe::MascotTagAnalyseMetricsCPP()
{
	UMyTemplateInt* LastEnterElevator = Cast<UMyTemplateInt>(MyMascot->GetSpecificKnowledge(PrisonerEnteredInElevator));
	if (LastEnterElevator)
	{
		APrisonerController* PC = Cast<APrisonerController>(MyMascot->GetPC());
		if (PC && PC->GetPersonalPlayerID() == LastEnterElevator->IntValue)
		{
			return EMascotTag::IsPrisonerEnteredInElevator;
		}
	}
	return EMascotTag::None;
}
