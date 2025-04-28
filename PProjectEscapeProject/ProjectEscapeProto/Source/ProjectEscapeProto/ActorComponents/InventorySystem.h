// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIHelpers.h"
#include "Components/ActorComponent.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "InventorySystem.generated.h"

class AItem;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventorySystemDelegate, AItem*, FirstItem, AItem*, SecondItem);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventorySystem();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(AItem* Item, APrisoner* Prisoner, bool AddToFirstSlot);
	UFUNCTION(Client, Reliable)
	void Client_AddItem(AItem* Item);

	UFUNCTION(BlueprintCallable)
	bool TryRemoveItem(AItem* Item, APrisoner* Prisoner, bool RemoveToFirstSlot, float DropDistance = 100);
	UFUNCTION(NetMulticast, Reliable)
	void AttachItem(AItem* Actor, APrisoner* Prisoner, bool IsInvisible);
	UFUNCTION(Server, Reliable)
	void Server_SetFirstItemSlot(AItem* Item);
	UFUNCTION(Server, Reliable)
	void Server_SetSecondItemSlot(AItem* Item);
	void DropInventory();

protected:
	virtual void BeginPlay() override;
	void AttachOnHand(AItem* Actor, APrisoner* Prisoner, USkeletalMeshComponent* ParentComponent, bool IsInvisible);

	UFUNCTION(NetMulticast, Reliable)
	void DetachItem(AItem* Item, APrisoner* Prisoner, float DropDistance = 100);
	UFUNCTION()
	void OnRep_FirstItemSlot();
	UFUNCTION()
	void OnRep_SecondItemSlot();
	UFUNCTION()
	void RefreshHandItem(APrisoner* Prisoner);

public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FirstItemSlot)
	AItem* FirstItemSlot;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SecondItemSlot)
	AItem* SecondItemSlot;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FInventorySystemDelegate OnInventoryChanged;

protected:
	TWeakObjectPtr<AItem> PrevisuItem;
	TWeakObjectPtr<APawn> MyOwner;
};
