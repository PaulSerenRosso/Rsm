// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionComponent.h"

#include "Engine/PackageMapClient.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/Utilities/MyBitReader.h"
#include "ProjectEscapeProto/Utilities/MyBitWriter.h"


// Sets default values for this component's propertie
UPlayerActionComponent::UPlayerActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerActionComponent::AskExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Server_ExecuteAction(Payload, Actors);
}

void UPlayerActionComponent::AskStopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Server_StopExecuteAction(Payload, Actors);
}

void UPlayerActionComponent::Server_ExecuteAction_Implementation(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if (!IsInit) return;
	MyServer_ExecuteAction(Payload, Actors);
	EventServerExecuteAction(Payload, Actors);
}


// Called when the game starts
void UPlayerActionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	UBinaryDataSubsystem* BinaryDataSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UBinaryDataSubsystem>();
	MyBitReader = BinaryDataSubsystem->BitReader;
	MyBitWriter = BinaryDataSubsystem->BitWriter;
}

bool UPlayerActionComponent::CanExecuteAction()
{
	return MyPrisoner->CanStartPlayersActions;
}

void UPlayerActionComponent::Multicast_StopExecuteAction_Implementation(const TArray<uint8>& Payload,
	const TArray<AActor*>& Actors)
{
	if (!IsInit) return;
	MyMulticast_StopExecuteAction(Payload, Actors);
	EventMulticastStopExecuteAction(Payload, Actors);
	OnPlayerActionExecuted.Broadcast(Payload, Actors);
}

void UPlayerActionComponent::Server_StopExecuteAction_Implementation(const TArray<uint8>& Payload,
	const TArray<AActor*>& Actors)
{
	if (!IsInit) return;
	MyServer_StopExecuteAction(Payload, Actors);
	EventServerStopExecuteAction(Payload, Actors);
}

void UPlayerActionComponent::MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
}

void UPlayerActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
}

void UPlayerActionComponent::MyMulticast_StopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
}

void UPlayerActionComponent::MyServer_StopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
}


void UPlayerActionComponent::Multicast_ExecuteAction_Implementation(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if (!IsInit) return;
	MyMulticast_ExecuteAction(Payload, Actors);
	EventMulticastExecuteAction(Payload, Actors);
	OnPlayerActionExecuted.Broadcast(Payload, Actors);
}


void UPlayerActionComponent::Server_InitAction_Implementation(APrisoner* InPrisoner)
{
	InitServerAction(InPrisoner);
}

void UPlayerActionComponent::Multicast_InitAction_Implementation(APrisoner* InPrisoner)
{
	InitMulticastAction(InPrisoner);
}

void UPlayerActionComponent::InitServerAction(APrisoner* InPrisoner)
{
	EventServerInitAction(InPrisoner);
	Multicast_InitAction(InPrisoner);
}

void UPlayerActionComponent::InitMulticastAction(APrisoner* InPrisoner)
{
	EventMulticastInitAction(InPrisoner);
	IsInit = true;
	MyPrisoner = InPrisoner;
}

// Called every frame
void UPlayerActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
