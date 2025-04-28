// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemActionComponent.h"

#include "ProjectEscapeProto/ActorComponents/InventorySystem.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"


// Sets default values for this component's properties
UDropItemActionComponent::UDropItemActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDropItemActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if (CanExecuteAction())
	{
		InventorySystem->TryRemoveItem(InventorySystem->FirstItemSlot, MyPrisoner, true);
		Super::MyServer_ExecuteAction(Payload, Actors);
		Multicast_ExecuteAction(Payload, Actors);
	}
}

void UDropItemActionComponent::InitServerAction(APrisoner* InPrisoner)
{
	Super::InitServerAction(InPrisoner);
	InventorySystem = InPrisoner->Inventory;
}

bool UDropItemActionComponent::CanExecuteAction()
{
	return Super::CanExecuteAction() && InventorySystem.IsValid() && InventorySystem->FirstItemSlot;
}
