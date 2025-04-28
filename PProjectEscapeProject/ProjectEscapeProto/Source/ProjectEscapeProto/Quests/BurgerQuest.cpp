// Fill out your copyright notice in the Description page of Project Settings.


#include "BurgerQuest.h"

void UBurgerQuest::OnQuestSuccess()
{
	OnQuestUpdated.Broadcast(0, -1);
	if (++CurrentCount >= MaxDelivery)
	{
		IsFinished = true;
		OnQuestFinished.Broadcast(this, true);
	}
}

FString UBurgerQuest::GetQuestProgress()
{
	FString QuestDescription = FString::FromInt(CurrentCount) + "/" + FString::FromInt(MaxDelivery);
	if (IsFinished) QuestDescription = "<Strike>" + QuestDescription + "</>";
	return QuestDescription;
}
