// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchItemActionComponent.h"

#include "ProjectEscapeProto/ActorComponents/InventorySystem.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"


// Sets default values for this component's properties
USwitchItemActionComponent::USwitchItemActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USwitchItemActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void USwitchItemActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if(CanExecuteAction())
	{
		Super::MyServer_ExecuteAction(Payload, Actors);
		AItem* Temp = InventorySystem->FirstItemSlot;
		InventorySystem->Server_SetFirstItemSlot(InventorySystem->SecondItemSlot);
		InventorySystem->Server_SetSecondItemSlot(Temp);
		InventorySystem->AttachItem(InventorySystem->FirstItemSlot, MyPrisoner, false);
		InventorySystem->AttachItem(InventorySystem->SecondItemSlot, MyPrisoner, true);
		InventorySystem->Client_AddItem(InventorySystem->FirstItemSlot);
		Multicast_ExecuteAction(Payload, Actors);
	}
	
}


void USwitchItemActionComponent::MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyMulticast_ExecuteAction(Payload, Actors);


	
}

void USwitchItemActionComponent::InitMulticastAction(APrisoner* InPrisoner)
{
	Super::InitMulticastAction(InPrisoner);
	InventorySystem = InPrisoner->Inventory;
}

// Called every frame
void USwitchItemActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
