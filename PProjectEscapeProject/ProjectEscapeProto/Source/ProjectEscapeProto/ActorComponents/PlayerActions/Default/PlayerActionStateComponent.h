// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerActionComponent.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "PlayerActionStateComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UPlayerActionStateComponent : public UPlayerActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerActionStateComponent();


	void AskStopAction(const TArray<uint8>& Payload = TArray<uint8>(),
	                   const TArray<AActor*>& Actors = TArray<AActor*>());
	UFUNCTION(Server, Reliable)
	void Server_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	UFUNCTION(BlueprintImplementableEvent)
	void EventMulticastStopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	UFUNCTION(BlueprintImplementableEvent)
	void EventServerStopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	bool IsActiveState() const { return ActiveState; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual EPlayerActionState GetPlayerActionState();
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void MyMulticast_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	virtual void MyServer_StopAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable)
	FPlayerActionDelegate OnPlayerActionStopped;
	bool ActiveState = false;
};
