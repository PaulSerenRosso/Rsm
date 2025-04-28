// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalyzerMainQuestStart.h"

#include "ProjectEscapeProto/Mascot/Mascot.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplateBool.h"

EMascotTag UAnalyzerMainQuestStart::MascotTagAnalyseMetricsCPP()
{
	UMyTemplateBool* IsStarted = Cast<UMyTemplateBool>(MyMascot->GetSpecificKnowledge(MainQuestStart));
	if (IsStarted && IsStarted->BoolValue) return EMascotTag::AsMainQuestStart;
	return EMascotTag::None;
}
