// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorSensor.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Managers/ElevatorManager.h"
#include "ProjectEscapeProto/Mascot/Mascot.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplateBool.h"
#include "ProjectEscapeProto/Utilities/MyTemplate/MyTemplateInt.h"

void UElevatorSensor::InitSensor(UWorld* WorldContext, AMascot* Mascot, APrisonerController * PC)
{
	Super::InitSensor(WorldContext, Mascot, PC);
	AOnlineGameMode* GM = Cast<AOnlineGameMode>(WorldContext->GetAuthGameMode());
	if (!GM) return;
	if (!GM->ElevatorManager.IsValid()) return;
	//TODO:faudra toucher à aç 
	//GM->ElevatorManager->OnPrisonerEnteredInElevator.AddDynamic(this, &UElevatorSensor::OnPrisonerEntered);
	//GM->ElevatorManager->OnElevatorClosed.AddDynamic(this, &UElevatorSensor::OnElevatorClose);
}

void UElevatorSensor::UpdateMetrics()
{
	Super::UpdateMetrics();
}

void UElevatorSensor::OnPrisonerEntered(int Index)
{
	if (!MyMascot) return;
	if (!MyPC.IsValid()) return;
	UMyTemplateInt* PrisonerEntered = NewObject<UMyTemplateInt>();
	PrisonerEntered->IntValue = Index;
	NbEnteredPrisonner++;

	UMyTemplateInt* NbEnteredPrisonnerTemplate = NewObject<UMyTemplateInt>();
	NbEnteredPrisonnerTemplate->IntValue = NbEnteredPrisonner;
	
	MyMascot->AddMetricsToKnowledge(PrisonerEnteredInElevator, PrisonerEntered);
	MyMascot->AddMetricsToKnowledge(NbPrisonerEnteredInElevator, NbEnteredPrisonnerTemplate);
}

void UElevatorSensor::OnElevatorClose()
{
	if (!MyMascot) return;
	if (!MyPC.IsValid()) return;
	UMyTemplateBool* TypeElevatorClosed = NewObject<UMyTemplateBool>();
	TypeElevatorClosed->BoolValue = true;
	MyMascot->AddMetricsToKnowledge(ElevatorClosed, TypeElevatorClosed);
}
