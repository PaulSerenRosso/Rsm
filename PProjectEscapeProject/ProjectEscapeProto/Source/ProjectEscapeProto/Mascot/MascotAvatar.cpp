// Fill out your copyright notice in the Description page of Project Settings.


#include "MascotAvatar.h"

UMascotAvatar::UMascotAvatar()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMascotAvatar::PrintLine_Implementation(const FString& TextToPrint, bool NeedToStayPrinted)
{
	MascotNeedToTalk(TextToPrint, NeedToStayPrinted);
}



void UMascotAvatar::InitToNewPhasis_Implementation(const FString& TextToPrint, const float PhaseTimer,
	const FString& QuestDescription, bool NeedToStayPrinted)
{
	BP_InitToNewPhasis(TextToPrint,  PhaseTimer, QuestDescription,
NeedToStayPrinted);
}

void UMascotAvatar::PrintLineAlone(const FString& TextToPrint, bool NeedToStayPrinted)
{
	MascotNeedToTalk(TextToPrint, NeedToStayPrinted);
}

void UMascotAvatar::BeginPlay()
{
	Super::BeginPlay();
}

void UMascotAvatar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

