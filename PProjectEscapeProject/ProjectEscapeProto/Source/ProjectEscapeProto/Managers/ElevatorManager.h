// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElevatorManager.generated.h"

class AOnlineGameMode;
class FPrisonnersInElevatorUpdatedDelegate;
class APrisoner;
class UElevatorManagerData;
class AElevator;


UCLASS()
class PROJECTESCAPEPROTO_API AElevatorManager : public AActor
{
	GENERATED_BODY()

public:
	
	// Sets default values for this actor's properties
	AElevatorManager();

protected:
	void AskGameStart();
	UFUNCTION()
	void OnGameReadyToStart();
	void OpenElevatorAtLastPhasis();
	UFUNCTION()
	void OnNewPhasisBegan(int CurrentPhasisIndex);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnStartGame();
	void AskGameEnd();
	UFUNCTION()
	void OnElevatorClosed();
	UFUNCTION()
	virtual void OnPreviousPhasisEnded(int CurrentPhasisIndex);
	
protected:
	AOnlineGameMode* OnlineGameMode;
	UPROPERTY(EditAnywhere)
	AElevator* Elevator;
	UPROPERTY(EditAnywhere)
	UElevatorManagerData* ElevatorManagerData;
	TArray<APrisoner*> Prisoners;
	//TODO: use this 
	FPrisonnersInElevatorUpdatedDelegate* OnPrisonnersInElevatorUpdatedDelegate;
	FTimerHandle TimerHandle;

public:

	
	
	
};
