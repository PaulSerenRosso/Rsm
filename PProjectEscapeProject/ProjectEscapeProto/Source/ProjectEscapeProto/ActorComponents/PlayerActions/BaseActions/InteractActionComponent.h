// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionComponent.h"
#include "InteractActionComponent.generated.h"

#define ECC_Interactable ECC_GameTraceChannel3

class UTweakeableValueCharacterData;
class UCameraComponent;
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHoverInteractableDelegate, bool, IsHit, FString, InteractionName, FString, ModeOfInputUtilisation);
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API UInteractActionComponent : public UPlayerActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractActionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void MyServer_StopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void BeginPlay() override;
	virtual void InitMulticastAction(APrisoner* InPrisoner) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FHoverInteractableDelegate OnHoverInteractable;
	TWeakObjectPtr<UCameraComponent> CameraComponent;
	TWeakObjectPtr<UTweakeableValueCharacterData> CharacterData;
	ECollisionChannel InteractableChannel;


protected:
	TWeakObjectPtr<AActor> LastInteractableActor = nullptr;
};
