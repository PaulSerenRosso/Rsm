// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "GeneratorQuest.generated.h"

class AGenerator;
class UGeneratorQuestData;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UGeneratorQuest : public UQuest
{
	GENERATED_BODY()

protected:
	virtual void StartQuest(UWorld* World) override;
	virtual void OnQuestFail() override;
	UFUNCTION()
	void OnGeneratorRepared(AActor* HitBy);
	UFUNCTION()
	void OnGeneratorTryToRepared(AActor* HitBy);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GeneratorQuest")
	UGeneratorQuestData* GeneratorQuestData;

	int UnachievedGenerators;
};
