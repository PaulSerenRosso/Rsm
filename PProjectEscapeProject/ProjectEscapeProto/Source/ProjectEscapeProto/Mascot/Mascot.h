// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "Mascot.generated.h"


class UMyTemplateInt;
class APrisonerController;
class UMascotAvatar;
class UMyTemplate;
class UMascotTagActionData;
class UMascotDependencyProvider;
class UMascotTagAction;
class UMascotMetricsAnalyzer;
class UMascotSensor;
class UMascotData;

UCLASS()
class PROJECTESCAPEPROTO_API AMascot : public AActor
{
	GENERATED_BODY()

public:
	AMascot();
	virtual void Tick(float DeltaSeconds) override;

	//UFUNCTION(Server, Reliable)
	void Init(UMascotData* NewData, APrisonerController* _PC);
	void OnMetricsRefreshed();
	void AddMetricsToKnowledge(EMascotKnowledgeType Tag, UMyTemplate *Metrics);
	void OnTagAnalyserFinished();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMyTemplate* GetSpecificKnowledge(EMascotKnowledgeType Tag);
	APrisonerController* GetPC() const { return PC; }

protected:
	virtual void BeginPlay() override;

public:
	UMascotDependencyProvider* DependencyProvider;

protected:
	UPROPERTY(BlueprintReadOnly)
	APrisonerController* PC;

private:
	UPROPERTY(EditAnywhere)
	UMascotData* MascotData;
	UPROPERTY()
	TMap<TEnumAsByte<EMascotKnowledgeType>, UMyTemplate*> Knowledge;
	TArray<EMascotTag> CurrentMascotTags;
	TArray<UMascotSensor*> Sensors;
	TArray<UMascotMetricsAnalyzer*> MetricsAnalyzers;
	TArray<UMascotTagAction*> TagActions;
};
