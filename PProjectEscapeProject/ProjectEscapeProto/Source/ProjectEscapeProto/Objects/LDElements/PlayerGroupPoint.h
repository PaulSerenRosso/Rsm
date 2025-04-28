// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectEscapeProto/Objects/SpawnActor.h"
#include "PlayerGroupPoint.generated.h"

class UBoxComponent;
class APrisoner;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerGroupUpdatedDelegate, APrisoner*, Player);

UCLASS()
class PROJECTESCAPEPROTO_API APlayerGroupPoint : public ASpawnActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerGroupPoint();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	UPROPERTY(BlueprintAssignable)
	FPlayerGroupUpdatedDelegate OnPlayerJoin;
	UPROPERTY(BlueprintAssignable)
	FPlayerGroupUpdatedDelegate OnPlayerLeave;
	UPROPERTY(BlueprintReadOnly)
	TArray<APrisoner*> PrisonersGrouped;

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* GroupColliderOverlap;
};
