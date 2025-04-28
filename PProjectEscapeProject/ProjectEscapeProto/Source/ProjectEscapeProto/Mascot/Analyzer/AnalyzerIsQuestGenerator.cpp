// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalyzerIsQuestGenerator.h"
#include "ProjectEscapeProto/Mascot/Mascot.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplateEnumTypeOfQuest.h"

EMascotTag UAnalyzerIsQuestGenerator::MascotTagAnalyseMetricsCPP()
{
	UMyTemplateEnumTypeOfQuest *TypeOfQuest = Cast<UMyTemplateEnumTypeOfQuest>(MyMascot->GetSpecificKnowledge(EMascotKnowledgeType::TypeOfQuest));
	// if (TypeOfQuest && TypeOfQuest->EnumValue == EQuestType::Generator)
	// 	return EMascotTag::IsGeneratorQuest;
	return EMascotTag::None;
}
