// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectEscapeProto/DataAssets/QuestManagerData.h"
#include "QuestManager.generated.h"

class AMapInfoManager;
class UQuest;
class UQuestManagerData;
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestDelegate, UQuest*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestsFinishedDelegate, bool, IsSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestsUpdateDelegate, FText, QuestProgress);
UCLASS()
class PROJECTESCAPEPROTO_API AQuestManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestManager();
	virtual void BeginPlay() override;
	UFUNCTION(Category = "QuestManager")
	void Init(UQuestManagerData* QuestManagerData);
	UFUNCTION(Category = "QuestManager", BlueprintImplementableEvent, BlueprintCallable)
	void InitCallback();
	UFUNCTION(Server, Reliable)
	void MulticastInitQuest(int Index, bool IsMainQuest);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastInit(UQuestManagerData* QuestManagerData);
	UFUNCTION(Server, Reliable)
	void Server_GetQuestsDescription();
	UFUNCTION(BlueprintCallable, Category = "QuestManager")
	void EndOfQuests(bool IsSuccess);
	float GetPercentageMainQuestsFinished();
	
protected:
	UFUNCTION()
	void OnQuestFinished(UQuest* Quest, bool IsSuccess);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestManager")
	void ShowQuestDescription();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "QuestManager")
	void HideQuestDescription();
	UFUNCTION()
	void QuestUpdated(int ScoreAmount, int PlayerId);
	void StartQuests();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartQuest();
	TArray<int> RefillAllQuestAvailable(TArray<UQuestData*> Quests);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateQuest(const FText &QuestProgress);

public:
	FQuestsFinishedDelegate OnQuestsFinished;
	FQuestDelegate OnQuestFailed;
	FQuestDelegate OnQuestStarted;
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FQuestsFinishedDelegate OnQuestUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FQuestsUpdateDelegate BP_OnQuestUpdated;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Quest")
	FText QuestsDescription;
	UPROPERTY(VisibleAnywhere)
	UQuestManagerData* CurrentQuestManagerData;
	

protected:
	const float QuestDescriptionShowedTime = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Quest")
	TArray<UQuest*> CurrentQuests;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quest")
	AMapInfoManager* InfoManager;
	
private:
	FTimerHandle QuestDescriptionShowedTimerHandle;
	TArray<int> QuestsIndex;
};


