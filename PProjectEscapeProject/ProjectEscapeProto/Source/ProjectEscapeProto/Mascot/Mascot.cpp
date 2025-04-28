// Fill out your copyright notice in the Description page of Project Settings.


#include "Mascot.h"

#include "MascotMetricsAnalyzer.h"
#include "MascotSensor.h"
#include "MascotTagAction.h"
#include "ProjectEscapeProto/Characters/PrisonerController.h"
#include "ProjectEscapeProto/DataAssets/MascotData.h"
#include "ProjectEscapeProto/DataAssets/MascotTagActionData.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplate.h"


AMascot::AMascot()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMascot::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

// Called when the game starts
void AMascot::BeginPlay()
{
	Super::BeginPlay();
}

void AMascot::Init(UMascotData* NewData, APrisonerController* _PC)
{
	if (!HasAuthority()) return;
	MascotData = NewData;
	PC = _PC;
	Knowledge.Empty();
	for (auto NewSensor : MascotData->MascotteSensors)
	{
		UMascotSensor* NewSensorInstance = 	Cast<UMascotSensor>(
		AddComponentByClass(NewSensor, false, FTransform::Identity, false));
		Sensors.Add(NewSensorInstance);
		NewSensorInstance->InitSensor(GetWorld(), this, _PC);
	}

	for (auto NewMetrics : MascotData->AllMetricsAnalyzer)
	{
		UMascotMetricsAnalyzer* NewMetricsAnalyzer = Cast<UMascotMetricsAnalyzer>(
		AddComponentByClass(NewMetrics, false, FTransform::Identity, false));
		MetricsAnalyzers.Add(NewMetricsAnalyzer);
		NewMetricsAnalyzer->Init(this);
	}

	for (auto NewActionTag : MascotData->AllTagActions)
	{
		UMascotTagAction* NewActionTagInstance = Cast<UMascotTagAction>(
		AddComponentByClass(NewActionTag, false, FTransform::Identity, false));
		NewActionTagInstance->PC = _PC;
		TagActions.Add(NewActionTagInstance);
	}
	
}

void AMascot::OnMetricsRefreshed()
{
	if (!HasAuthority()) return;
	CurrentMascotTags.Empty();
	for (auto Analyzer : MetricsAnalyzers)
	{
		if (!Analyzer || !Analyzer->IsValidLowLevel()) continue;
		
		EMascotTag ReturnedType = Analyzer->MascotTagAnalyseMetrics();
		if (ReturnedType && ReturnedType != EMascotTag::None)
		{
			CurrentMascotTags.Add(ReturnedType);
		}
	}
	OnTagAnalyserFinished();
}

void AMascot::AddMetricsToKnowledge(EMascotKnowledgeType Tag, UMyTemplate *Metrics)
{
	if (!HasAuthority()) return;
		if (Knowledge.Contains(Tag))
			Knowledge[Tag] = Metrics;
		else
			Knowledge.Add(Tag, Metrics);
		OnMetricsRefreshed();
}

void AMascot::OnTagAnalyserFinished()
{
	if (!HasAuthority()) return;
	for (auto Tag : CurrentMascotTags)
	{
		for (int i = 0; i < TagActions.Num(); i++)
		{
			if (TagActions[i]->InputMascotTag.Contains(Tag))
			{
				UMascotTagAction* NewAction = TagActions[i];
				NewAction->Execute();
				if (NewAction->ConsumeTagAction)
				{
					TagActions.RemoveAt(i);
					--i;
				}
			}
		}
	}
}

UMyTemplate* AMascot::GetSpecificKnowledge(EMascotKnowledgeType Tag)
{
	if (Knowledge.Contains(Tag) && IsValid(Knowledge[Tag]))
	{
		return Knowledge[Tag];
	}
	return nullptr;
}
