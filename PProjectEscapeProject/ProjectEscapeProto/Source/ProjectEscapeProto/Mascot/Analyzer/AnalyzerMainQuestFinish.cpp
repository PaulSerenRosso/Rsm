// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalyzerMainQuestFinish.h"

#include "ProjectEscapeProto/Mascot/Mascot.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplateBool.h"

EMascotTag UAnalyzerMainQuestFinish::MascotTagAnalyseMetricsCPP()
{
	UMyTemplateBool* IsFinish = Cast<UMyTemplateBool>(MyMascot->GetSpecificKnowledge(AllQuestsEnded));
	if (IsFinish)
		return (IsFinish->BoolValue) ? EMascotTag::MainQuestSuccess : EMascotTag::MainQuestFail;
	return EMascotTag::None;
}
