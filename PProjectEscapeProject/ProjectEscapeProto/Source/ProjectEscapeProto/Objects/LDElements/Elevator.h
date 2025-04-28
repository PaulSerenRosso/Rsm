// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Elevator.generated.h"
class APrisoner;
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPrisonnersInElevatorUpdatedDelegate, TArray<APrisoner*>, PrisonersGrouped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FElevatorDelegate);
class APlayerGroupPoint;
class UBoxComponent;

UCLASS()
class PROJECTESCAPEPROTO_API AElevator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElevator();
	

protected:
	UFUNCTION()
	void OnPrisonerLeavedElevator(APrisoner* Player);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnPrisonerEnteredElevator(APrisoner* Prisoner);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Multicast_CloseElevator();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Multicast_OpenElevator();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Multicast_CloseImmediately();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Multicast_StartWaitingTime();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Multicast_StopWaitingTime();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Server_CloseElevator();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Server_OpenElevator();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Server_CloseImmediately();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Server_StartWaitingTime();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Server_StopWaitingTime();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Server, Reliable)
	void Server_CloseElevator();
	UFUNCTION(Server, Reliable)
	void Server_OpenElevator();
	UFUNCTION(Server, Reliable)
	void Server_CloseElevatorImmediately();
	
	UFUNCTION(Server, Reliable)
	void Server_StartWaitingTime();
	UFUNCTION(Server, Reliable)
	void Server_StopWaitingTime();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CloseElevator();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OpenElevator();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CloseElevatorImmediately();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartWaitingTime();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopWaitingTime();
public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FElevatorDelegate OnElevatorClosed;
	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FElevatorDelegate OnElevatorOpened;
	FElevatorDelegate OnElevatorPlayersUpdated;
	FPrisonnersInElevatorUpdatedDelegate OnPrisonersInElevatorUpdated;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerGroupPoint* PlayerGroupPoint;
	UPROPERTY(EditAnywhere)
	UChildActorComponent* PlayerGroupActorComponent;


};
