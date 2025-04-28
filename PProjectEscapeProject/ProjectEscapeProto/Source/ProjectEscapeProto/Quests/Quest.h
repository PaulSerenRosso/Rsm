// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Quest.generated.h"

class ASpawnActor;
class AItem;
class APrisonerController;
class UQuestData;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuestFinishedDelegate, UQuest*, Quest, bool, IsSuccess);
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuestUpdatedDelegate, int, scoreAmount, int, playerId);

UCLASS(Blueprintable)
class PROJECTESCAPEPROTO_API UQuest : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void StartQuest(UWorld* World);
	virtual void OnQuestFail();
	UFUNCTION()
	virtual void OnQuestSuccess();
	void EndOfQuest();
	virtual FText GetQuestDescription();
	UFUNCTION(BlueprintCallable)
	virtual FString GetQuestProgress();
	
public:
	bool IsStarted = false;
	bool IsFinished;
	FQuestFinishedDelegate OnQuestFinished;
	FQuestUpdatedDelegate OnQuestUpdated;
	UPROPERTY(BlueprintReadOnly, Category="Quest")
	UQuestData* QuestData;
	UWorld* WorldRef;
	bool IsMainQuest = false;
	

protected:
	//TArray<ASpawnActor*> ActorSpawnedArray;
	TArray<TWeakObjectPtr<ASpawnActor>> ActorSpawnedArray;
	int MaxCount = 0;
	int CurrentCount = 0;
};
