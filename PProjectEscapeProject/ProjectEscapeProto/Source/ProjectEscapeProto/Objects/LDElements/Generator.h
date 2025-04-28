// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectEscapeProto/Interfaces/Hitable.h"
#include "ProjectEscapeProto/Objects/SpawnActor.h"
#include "Generator.generated.h"

class UWidgetComponent;
class UGeneratorData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReparedDelegate, AActor*, HitBy);


UCLASS()
class PROJECTESCAPEPROTO_API AGenerator : public ASpawnActor, public IIHitable
{
	GENERATED_BODY()

public:
	AGenerator();
	virtual void Tick(float DeltaTime) override;
	virtual bool CanHit(AActor* Actor) override;
	virtual bool TryHit(AActor* Actor) override;

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent, Category = "Generator")
	void RefreshWidget();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintAssignable,  Category = "GeneratorEvent")
	FOnReparedDelegate OnRepared;
	UPROPERTY(BlueprintAssignable, Category = "GeneratorEvent")
	FOnReparedDelegate OnTryToRepared;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generator")
	UGeneratorData* GeneratorData;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Generator")
	int CurrentStep;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generator")
	UWidgetComponent* GeneratorWidget;
};
