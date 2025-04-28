// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AniversaryCakeDestination.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTESCAPEPROTO_API AAniversaryCakeDestination : public AActor
{
	GENERATED_BODY()
	
public:	
	AAniversaryCakeDestination();
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnCakeArrived(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AniversaryCakeDestination")
	UBoxComponent* BoxComponent;

};
