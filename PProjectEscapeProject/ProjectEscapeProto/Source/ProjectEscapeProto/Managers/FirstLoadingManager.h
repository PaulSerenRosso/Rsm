// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstLoadingManager.generated.h"

class UOnlineGameVivoxSubsystem;

UCLASS()
class PROJECTESCAPEPROTO_API AFirstLoadingManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFirstLoadingManager();

protected:
	// Called when the game starts or when spawned
	TWeakObjectPtr<UOnlineGameVivoxSubsystem> OnlineGameVivoxSubsystem;
	UFUNCTION()
	void OnVivoxInitialized();
	UFUNCTION(BlueprintCallable)
	void BeginInitCPP();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_BeginInit();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_EndInit();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	bool NeedToInitVivox;
};
