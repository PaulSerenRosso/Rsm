// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/Objects/Items/Item.h"
#include "DanceFloor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDancefloorFinishedEvent);

class UTextRenderComponent;
class AOnlineGameMode;
struct FActionTimer;
class UDancefloorData;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API ADanceFloor : public AItem
{
	GENERATED_BODY()
	
	ADanceFloor();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void TickReward(int Id);
virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintAssignable, Category = "DanceFloor")
	FDancefloorFinishedEvent OnDancefloorFinished;
	UPROPERTY(BlueprintAssignable, Category = "DanceFloor")
	FDancefloorFinishedEvent OnDancefloorStarted;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDancefloorData* DancefloorData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DanceFloor")
	UTextRenderComponent* TextRender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DanceFloor")
	UStaticMeshComponent* FloorMesh;
	
	TArray<int32> PlayerInteracting;
	TMap<int32, float> PlayerInteractingTimer;
	char DanseStarted = '0'; // 0 = not started, 1 = started, 2 = finished
	float TimerBeforeFinish = 0;
	TWeakObjectPtr<AOnlineGameMode> GM = nullptr;
	UPROPERTY(Replicated)
	FString CurrentTimerText = "";
	
};
