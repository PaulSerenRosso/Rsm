// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateSensor.h"

#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/DataAssets/QuestData.h"
#include "ProjectEscapeProto/Mascot/Mascot.h"
#include "ProjectEscapeProto/Quests/Quest.h"
#include "ProjectEscapeProto/Quests/QuestManager.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplateBool.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplateEnumTypeOfQuest.h"

void UGameStateSensor::InitSensor(UWorld* WorldContext, AMascot* Mascot, APrisonerController* PC)
{
	Super::InitSensor(WorldContext, Mascot, PC);
	AOnlineGameMode* GM = Cast<AOnlineGameMode>(WorldContext->GetAuthGameMode());
	if (!GM) return;
	if (IsValid(GM) && IsValid(this))
	{
		if (!GM->QuestManager.IsValid()) return;
		// GM->QuestManager->OnQuestStarted.AddDynamic(this, &UGameStateSensor::StartQuest);
		// GM->OnQuestsFinished.AddDynamic(this, &UGameStateSensor::QuestsTerminated);
	}
}

void UGameStateSensor::UpdateMetrics()
{
	Super::UpdateMetrics();
}

void UGameStateSensor::StartQuest(UQuest* Quest)
{
	UMyTemplateBool* StartQ = NewObject<UMyTemplateBool>();
	StartQ->BoolValue = true;
	if (MyMascot)
		MyMascot->AddMetricsToKnowledge(MainQuestStart, StartQ);
	UMyTemplateEnumTypeOfQuest* TypeOfQuest = NewObject<UMyTemplateEnumTypeOfQuest>();
	// TypeOfQuest->EnumValue = Quest->QuestData->EnumQuestType;
}

void UGameStateSensor::QuestsTerminated(bool IsSuccess)
{
	UMyTemplateBool* AreSuccess = NewObject<UMyTemplateBool>();
	AreSuccess->BoolValue = IsSuccess;
	if (MyMascot)
		MyMascot->AddMetricsToKnowledge(AllQuestsEnded, AreSuccess);
	
	AOnlineGameMode* GM = Cast<AOnlineGameMode>(MyWordContext->GetAuthGameMode());
	if (!GM) return;
	if (IsValid(GM) && IsValid(this))
	{
		if (!GM->QuestManager.IsValid()) return;
		// GM->QuestManager->OnQuestStarted.RemoveDynamic(this, &UGameStateSensor::StartQuest);
		// GM->OnQuestsFinished.RemoveDynamic(this, &UGameStateSensor::QuestsTerminated);
	}
}
