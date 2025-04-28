// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabItemActionComponent.h"
#include "ProjectEscapeProto/ActorComponents/InventorySystem.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/Objects/Items/Item.h"


// Sets default values for this component's properties
UGrabItemActionComponent::UGrabItemActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabItemActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGrabItemActionComponent::Server_ExecuteAction_Implementation(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if(!CanExecuteAction()) return;
	GrabItem = Cast<AItem>(Actors[0]);
	Super::Server_ExecuteAction_Implementation(Payload, Actors);
	
	Multicast_ExecuteAction(Payload, Actors);
}

void UGrabItemActionComponent::InitServerAction(APrisoner* InPrisoner)
{
	Super::InitServerAction(InPrisoner);
	InventorySystem = InPrisoner->Inventory;
}

float UGrabItemActionComponent::GetCurrentTime()
{
	return 0.5f;
}

void UGrabItemActionComponent::EndTimerAction()
{
	Super::EndTimerAction();
}

void UGrabItemActionComponent::Server_StopAction_Implementation(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::Server_StopAction_Implementation(Payload, Actors);
	Multicast_StopAction(Payload, Actors);
	if (InventorySystem->FirstItemSlot == GrabItem || InventorySystem->SecondItemSlot == GrabItem) return;
	InventorySystem->TryAddItem(GrabItem, MyPrisoner, InventorySystem->FirstItemSlot == nullptr);
}

EPlayerActionState UGrabItemActionComponent::GetPlayerActionState()
{
	return EPlayerActionState::IsGrabbingItem;
}

// Called every frame
void UGrabItemActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

