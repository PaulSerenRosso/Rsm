// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ProjectEscapeProto/IA/EntityController.h"
#include "ProjectEscapeProto/Managers/MapInfoManager.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "ProjectEscapeProto/Utilities/StructUtilities.h"
#include "HearingNeighbourController.generated.h"

class UUltimateSprintTask;
class AAIDoorPathfinder;
class ATransitionGoalPathfinder;
class AMapInfoManager;
class APrisoner;
class UAISenseConfig_Sight;
class UHearingNeighbourData;
struct FAIStimulus;
class UAISenseConfig_Hearing;
class UAIPerceptionComponent;

UCLASS()
class PROJECTESCAPEPROTO_API AHearingNeighbourController : public AEntityController
{
	GENERATED_BODY()

public:
	AHearingNeighbourController();
	bool CanSeePlayer() const { return (PlayerTarget != nullptr); }
	APrisoner* GetPlayer() const { return PlayerTarget; }
	TArray<AActor*> GetRoomLocations(ERoomType Room);
	AActor* GetFirstRoomDoor(ERoomType NextRoomTag, ERoomType CurrentRoomTag);
	ERoomType GetNextRoomID(ERoomType Room);
	void SetCurrentRoom(ERoomType Room) { CurrentRoom = Room; }
	char SetCurrentPatrolLocations(ERoomType Room);
	FVector GetFirstPatrolLocation(bool NeedToRemove = true);
	int GetPatrolPointsCount() const { return CurrentPatrolLocations.Num(); }
	UHearingNeighbourData* GetData() const { return NeighbourEntityData; }
	void SwapState(EEntityState NewState);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
	void OnRoomOpened(ERoomType RoomUpdated);
	UFUNCTION()
	void OnRoomClosed(ERoomType RoomUpdated);
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	void SwitchToNewTarget();

private:
	float CalcStrengthFromDistanceAndStrength(float Distance, float Strength) const;
	UFUNCTION()
	void ClearTarget(int32 Index);

	void ComputePath();
	bool FindValidRound(ERoomType Current, TArray<ERoomType>& Path, TSet<ERoomType>& Visited, TSet<ERoomType>& Excluded);
	TArray<ERoomType> GetValidRound(TSet<ERoomType> Excluded);
	
public:
	float LastHearingSoundStrength = 0.f;
	AActor *LastHearingSoundActor = nullptr;
	ERoomType CurrentRoom = ERoomType::RSM; // Default room is 5 (RSM)
	
protected:
	EEntityState CurrentState = EEntityState::Patrol;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UHearingNeighbourData* NeighbourEntityData;
	APrisoner *PlayerTarget = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	TArray<TWeakObjectPtr<APrisoner>> PlayerCantBeHeard;
	AMapInfoManager* InfoManager;

	TArray<FVector> CurrentPatrolLocations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TMap<ERoomType, FString> RoomWaypoints;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FString DoorsActorsTag = "PatrolDoor";
	TArray<TWeakObjectPtr<AAIDoorPathfinder>> AllDoorsActors;

	float CurrentModifierSight = 0.0f;
	float CurrentModifierHearing = 0.0f;
	float CurrentTimerBeforeCanSwitchToTarget = 0.0f;

	TMap<ERoomType, TArray<ERoomType>> Graph;
	TArray<ERoomType> ValidPath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TSet<ERoomType> Exclusions;

	TWeakObjectPtr<UUltimateSprintTask> UltimateSprintTask;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TArray<APrisoner*> PlayersInSight;
};
