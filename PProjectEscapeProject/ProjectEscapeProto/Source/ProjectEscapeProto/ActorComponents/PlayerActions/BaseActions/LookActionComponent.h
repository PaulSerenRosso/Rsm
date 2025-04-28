// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/ActorComponents/PlayerActions/Default/PlayerActionComponent.h"
#include "LookActionComponent.generated.h"


class UTweakeableValueCharacterData;
class UCameraComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPEPROTO_API ULookActionComponent : public UPlayerActionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULookActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors) override;
	virtual void InitMulticastAction(APrisoner* InPrisoner) override;
protected:
	
	TWeakObjectPtr<UCameraComponent> CameraComponent = nullptr;
	TWeakObjectPtr<UTweakeableValueCharacterData> TweakeableValueCharacterData = nullptr;
	
};
