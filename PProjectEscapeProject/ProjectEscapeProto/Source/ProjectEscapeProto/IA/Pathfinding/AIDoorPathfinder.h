// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "AIDoorPathfinder.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTESCAPEPROTO_API AAIDoorPathfinder : public AActor
{
	GENERATED_BODY()

public:
	AAIDoorPathfinder();
	virtual void Tick(float DeltaTime) override;
	void GetTransitionTags(TArray<ERoomType> &From,ERoomType &To ) const { From = LinkedRoom; To = Room; }

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void SwitchRoom(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERoomType Room;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ERoomType> LinkedRoom = {ERoomType::RSM};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;
};
