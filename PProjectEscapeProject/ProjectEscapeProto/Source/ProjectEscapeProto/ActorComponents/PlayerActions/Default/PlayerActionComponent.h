// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectEscapeProto/Subsystem/BinaryDataSubsystem.h"
#include "PlayerActionComponent.generated.h"


class UMyBitWriter;
class UMyBitReader;
class UMyTemplate;
class APrisoner;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerActionDelegate, const TArray<uint8>&, Payload, const TArray<AActor*>&, Actors);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UPlayerActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerActionComponent();
	
	
	virtual void AskExecuteAction(const TArray<uint8>& Payload = TArray<uint8>(), const TArray<AActor*>& Actors = TArray<AActor*>());
	virtual void AskStopExecuteAction(const TArray<uint8>& Payload = TArray<uint8>(), const TArray<AActor*>& Actors = TArray<AActor*>());

	UFUNCTION(BlueprintImplementableEvent)
	void EventMulticastInitAction(APrisoner* InPrisoner);
	UFUNCTION(BlueprintImplementableEvent)
	void EventServerInitAction(APrisoner* InPrisoner);
	UFUNCTION(Server, Reliable)
	void Server_InitAction(APrisoner* InPrisoner);
	virtual void InitServerAction(APrisoner* InPrisoner);
	virtual void InitMulticastAction(APrisoner* InPrisoner);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_InitAction(APrisoner* InPrisoner);
	UPROPERTY(BlueprintAssignable)
	FPlayerActionDelegate OnPlayerActionExecuted;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual bool CanExecuteAction();
	UFUNCTION(BlueprintImplementableEvent)
	void EventMulticastExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	UFUNCTION(BlueprintImplementableEvent)
	void EventServerExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	UFUNCTION(BlueprintImplementableEvent)
	void EventMulticastStopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	UFUNCTION(BlueprintImplementableEvent)
	void EventServerStopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	UFUNCTION(Server, Reliable )
	void Server_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	UFUNCTION(Server, Reliable )
	void Server_StopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);

	virtual void MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	virtual void MyMulticast_StopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
	virtual void MyServer_StopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors);
protected:
	TWeakObjectPtr<UMyBitWriter> MyBitWriter;
	TWeakObjectPtr<UMyBitReader> MyBitReader;
	bool IsInit = false;
	

public:
	APrisoner *MyPrisoner;

};
