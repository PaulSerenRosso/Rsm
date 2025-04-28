// Fill out your copyright notice in the Description page of Project Settings.


#include "HearingNeighbourController.h"

#include "AHearingNeighbourCharacter.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/HearingNeighbourData.h"
#include "ProjectEscapeProto/IA/AICharacter.h"
#include "ProjectEscapeProto/IA/Pathfinding/AIBornPathFinder.h"
#include "ProjectEscapeProto/IA/Pathfinding/AIDoorPathfinder.h"
#include "ProjectEscapeProto/IA/Tasks/UltimateSprintTask.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"

AHearingNeighbourController::AHearingNeighbourController()
{
	PrimaryActorTick.bCanEverTick = true;
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(UAISense_Hearing::StaticClass());


	AIPerceptionComponent->OnTargetPerceptionUpdated.
	                       AddDynamic(this, &AHearingNeighbourController::OnPerceptionUpdated);
}

void AHearingNeighbourController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCantBeHeard.Empty();

	if (NeighbourEntityData)
	{
		if (IsValid(HearingConfig))
		{
			HearingConfig->HearingRange = NeighbourEntityData->HearingRange;
			HearingConfig->SetMaxAge(5.0f);
		}
		if (IsValid(SightConfig))
		{
			SightConfig->SightRadius = NeighbourEntityData->SightRadius;
			SightConfig->LoseSightRadius = NeighbourEntityData->LoseSightRadius;
			SightConfig->PeripheralVisionAngleDegrees = NeighbourEntityData->PeripheralVisionAngleDegrees;
		}
	}
	AIPerceptionComponent->RequestStimuliListenerUpdate();


	Graph.Add(ERoomType::Mall, {ERoomType::Suburbs, ERoomType::GasStation, ERoomType::RSM});
	Graph.Add(ERoomType::Suburbs, {ERoomType::Mall, ERoomType::Office, ERoomType::RSM});
	Graph.Add(ERoomType::Office, {ERoomType::Suburbs, ERoomType::GasStation, ERoomType::RSM});
	Graph.Add(ERoomType::GasStation, {ERoomType::Mall, ERoomType::Office, ERoomType::RSM});
	Graph.Add(ERoomType::RSM, {ERoomType::Mall, ERoomType::Suburbs, ERoomType::Office, ERoomType::GasStation});

	AOnlineGameMode* GM = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->OnRoomOpened.AddDynamic(this, &AHearingNeighbourController::OnRoomOpened);
		GM->OnRoomClosed.AddDynamic(this, &AHearingNeighbourController::OnRoomClosed);

		Exclusions.Empty();

		TArray<ERoomType> RoomsOpened = GM->GetRoomsOpened();
		TArray<ERoomType> AllRooms = UFunctionsUtilities::GetAllRoomTypes();

		for (ERoomType RoomType : AllRooms)
		{
			if (!RoomsOpened.Contains(RoomType))
			{
				Exclusions.Add(RoomType);
			}
		}
	}

	ComputePath();
}



TArray<AActor*> AHearingNeighbourController::GetRoomLocations(ERoomType Room)
{
	if (!InfoManager)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMapInfoManager::StaticClass(), FoundActors);
		if (FoundActors.Num() == 0) return TArray<AActor*>();
		InfoManager = Cast<AMapInfoManager>(FoundActors[0]);
	}

	return InfoManager->GetActorWithTag(RoomWaypoints[Room]);
}

AActor* AHearingNeighbourController::GetFirstRoomDoor(ERoomType NextRoomTag, ERoomType CurrentRoomTag)
{
	if (!InfoManager)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMapInfoManager::StaticClass(), FoundActors);
		if (FoundActors.Num() == 0) return nullptr;
		InfoManager = Cast<AMapInfoManager>(FoundActors[0]);
	}

	if (AllDoorsActors.Num() == 0)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIDoorPathfinder::StaticClass(), FoundActors);
		for (auto Actor : FoundActors)
		{
			AllDoorsActors.Add(Cast<AAIDoorPathfinder>(Actor));
		}
	}

	for (auto DoorsActor : AllDoorsActors)
	{
		TArray<ERoomType> From;
		ERoomType To;
		DoorsActor->GetTransitionTags(From, To);

		if (From.Contains(CurrentRoomTag) && To == NextRoomTag)
		{
			return DoorsActor.Get();
		}
	}
	return nullptr;
}

ERoomType AHearingNeighbourController::GetNextRoomID(ERoomType Room)
{
	if (ValidPath.Num() == 0) return ERoomType::RSM;
	for (int i = 0; i < ValidPath.Num(); i++)
	{
		if (ValidPath[i] == Room)
		{
			return ValidPath[(i == ValidPath.Num() - 1) ? 0 : i + 1];
		}
	}

	return ERoomType::RSM;
}

char AHearingNeighbourController::SetCurrentPatrolLocations(ERoomType Room)
{
	FVector Start = GetPawn()->GetActorLocation();
	AActor* Door = GetFirstRoomDoor(GetNextRoomID(Room), Room);

	if (!Door)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: No valid door found!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: No valid door found!"));
		return 0;
	}

	FVector End = Door->GetActorLocation();
	CurrentPatrolLocations.Empty();

	FVector Current = Start;
	FVector GlobalDirection = (End - Start).GetSafeNormal();

	TArray<AActor*> Locations = GetRoomLocations(Room);

	if (Locations.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: No valid locations found!"));
		return 0;
	}
	TArray<AAIBornPathFinder*> LocationsWayPoints;

	for (auto Location : Locations)
	{
		AAIBornPathFinder* PathFinder = Cast<AAIBornPathFinder>(Location);
		if (PathFinder)
			LocationsWayPoints.Add(PathFinder);
	}

	if (LocationsWayPoints.Num() == 0) return 0;

	AAIBornPathFinder::SortArrayByCost(LocationsWayPoints);

	const int MinimalCost = Cast<AAIBornPathFinder>(LocationsWayPoints[0])->GetCost();
	const int MinPoints = 3;

	TArray<AAIBornPathFinder*> CanBeSelectedPoints;

	for (int i = 0; i < LocationsWayPoints.Num(); i++)
	{
		AAIBornPathFinder* PathFinder = Cast<AAIBornPathFinder>(LocationsWayPoints[i]);
		if (PathFinder->GetCost() <= MinimalCost)
		{
			CanBeSelectedPoints.Add(PathFinder);
			LocationsWayPoints.RemoveAt(i);
			i--;
			continue;
		}
		if (CanBeSelectedPoints.Num() >= MinPoints) break;
		CanBeSelectedPoints.Add(PathFinder);
	}
	int PointsAdded = 0;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	while (CanBeSelectedPoints.Num() > 0)
	{
		AAIBornPathFinder* BestCandidate = nullptr;
		float BestScore = -FLT_MAX;
		int BestIndex = -1;

		for (int i = 0; i < CanBeSelectedPoints.Num(); i++)
		{
			AAIBornPathFinder* Candidate = CanBeSelectedPoints[i];
			FVector ToCandidate = Candidate->GetActorLocation() - Current;
			float DirectionProgress = FVector::DotProduct(ToCandidate.GetSafeNormal(), GlobalDirection);

			if (DirectionProgress < 0.2f)
			{
				continue;
			}

			FNavLocation NavStart, NavEnd;

			if (!NavSystem->ProjectPointToNavigation(Candidate->GetActorLocation(), NavStart) ||
				!NavSystem->ProjectPointToNavigation(End, NavEnd))
			{
				return 0;
			}

			UNavigationPath* NavPath = NavSystem->FindPathToLocationSynchronously(
				GetWorld(), NavStart.Location, NavEnd.Location);
			if (!NavPath || !NavPath->IsValid())
			{
				return 0;
			}

			float DistanceToEnd = NavPath->GetPathLength();

			float Score = -DistanceToEnd + (DirectionProgress * 200.0f);

			if (Score > BestScore)
			{
				BestScore = Score;
				BestCandidate = Candidate;
				BestIndex = i;
			}
		}

		if (BestIndex == -1)
		{
			break;
		}

		CurrentPatrolLocations.Add(BestCandidate->GetActorLocation());
		Current = BestCandidate->GetActorLocation();
		BestCandidate->IncreaseCost();
		CanBeSelectedPoints.RemoveAt(BestIndex);
		PointsAdded++;
	}

	while (PointsAdded < MinPoints && CanBeSelectedPoints.Num() > 0)
	{
		int NearestIndex = -1;
		float NearestDist = FLT_MAX;

		for (int i = 0; i < CanBeSelectedPoints.Num(); i++)
		{
			float Dist = FVector::Dist(CanBeSelectedPoints[i]->GetActorLocation(), Current);
			if (Dist < NearestDist)
			{
				NearestDist = Dist;
				NearestIndex = i;
			}
		}

		if (NearestIndex != -1)
		{
			FVector ForcedPoint = CanBeSelectedPoints[NearestIndex]->GetActorLocation();
			CurrentPatrolLocations.Add(ForcedPoint);
			CanBeSelectedPoints[NearestIndex]->IncreaseCost();
			CanBeSelectedPoints.RemoveAt(NearestIndex);
			PointsAdded++;
		}
	}


	TArray<FVector> OrderedPath;
	OrderedPath.Add(End); // On commence par la fin
	Current = End;

	while (CurrentPatrolLocations.Num() > 0)
	{
		int BestIndex = -1;
		float BestDistance = FLT_MAX;

		for (int i = 0; i < CurrentPatrolLocations.Num(); i++)
		{
			float Distance = FVector::Dist(Current, CurrentPatrolLocations[i]);

			if (Distance < BestDistance) // 🔥 On prend le point le plus proche du dernier ajouté
			{
				BestDistance = Distance;
				BestIndex = i;
			}
		}

		if (BestIndex != -1)
		{
			OrderedPath.Add(CurrentPatrolLocations[BestIndex]); // On l'ajoute à la liste
			Current = CurrentPatrolLocations[BestIndex]; // Il devient le nouveau "dernier" point
			CurrentPatrolLocations.RemoveAt(BestIndex); // On le supprime de la liste des points à traiter
		}
	}

	//OrderedPath.Add(Start); // On termine par le départ

	// Inverser la liste pour avoir le chemin dans le bon ordre (de Start vers End)
	Algo::Reverse(OrderedPath);
	CurrentPatrolLocations = OrderedPath;


	return CurrentPatrolLocations.Num();
}


FVector AHearingNeighbourController::GetFirstPatrolLocation(bool NeedToRemove)
{
	if (CurrentPatrolLocations.Num() == 0) return FVector::ZeroVector;
	FVector Location = CurrentPatrolLocations[0];
	if (NeedToRemove)
	{
		CurrentPatrolLocations.RemoveAt(0);
	}
	return Location;
}

void AHearingNeighbourController::SwapState(EEntityState NewState)
{
	if (CurrentState == NewState) return;
	if (NewState == EEntityState::Hunt)
	{
		if (!UltimateSprintTask.IsValid())
		{
			UltimateSprintTask = NewObject<UUltimateSprintTask>();
			UltimateSprintTask->Execute(this, Cast<AAICharacter>(GetPawn()), NeighbourEntityData);
		}
		AAHearingNeighbourCharacter* NeighbourCharacter = Cast<AAHearingNeighbourCharacter>(GetPawn());
		if (NeighbourCharacter && !NeighbourCharacter->IsInMonsterMode())
			NeighbourCharacter->SwitchToHumanMonsterMode(true);
	}
	if (NewState == EEntityState::Patrol && CurrentState == EEntityState::Chase)
	{
		if (UltimateSprintTask.IsValid())
		{
			UltimateSprintTask.Reset();
		}
		AAHearingNeighbourCharacter* NeighbourCharacter = Cast<AAHearingNeighbourCharacter>(GetPawn());
		if (NeighbourCharacter)
			NeighbourCharacter->OnEndAlert(PlayerTarget);
		if (NeighbourCharacter && NeighbourCharacter->IsInMonsterMode())
			NeighbourCharacter->SwitchToHumanMonsterMode(false);
	}
	CurrentState = NewState;
}

void AHearingNeighbourController::OnRoomOpened(ERoomType RoomUpdated)
{
	Exclusions.Remove(RoomUpdated);
	ComputePath();
}

void AHearingNeighbourController::OnRoomClosed(ERoomType RoomUpdated)
{
	Exclusions.Add(RoomUpdated);
	ComputePath();
}

float AHearingNeighbourController::CalcStrengthFromDistanceAndStrength(float Distance, float Strength) const
{
	float RatioDistance = HearingConfig->HearingRange - Distance;
	return Strength + RatioDistance;
}

void AHearingNeighbourController::ClearTarget(int32 PlayerId)
{
	LastHearingSoundStrength = 0.f;
	ClearFocus(EAIFocusPriority::Gameplay);
	AAHearingNeighbourCharacter* NeighbourCharacter = Cast<AAHearingNeighbourCharacter>(GetPawn());
	if (NeighbourCharacter)
	{
		NeighbourCharacter->OnLostSighPlayer(PlayerTarget);
	}
	PlayerTarget = nullptr;
}

void AHearingNeighbourController::ComputePath()
{
	ValidPath = GetValidRound(Exclusions);

	if (ValidPath.Num() > 0)
	{
		FString PathString;
		for (ERoomType Room : ValidPath)
		{
			FString RoomName = UEnum::GetValueAsString(Room);
			PathString.Append(RoomName);
			PathString.Append(TEXT(" "));
		}
		UE_LOG(LogTemp, Warning, TEXT("ronde trouvée : %s"), *PathString);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Aucune ronde valide trouvée."));
	}
}

bool AHearingNeighbourController::FindValidRound(ERoomType Current, TArray<ERoomType>& Path, TSet<ERoomType>& Visited,
                                                 TSet<ERoomType>& Excluded)
{
	int TotalRooms = 4 - Excluded.Num();

	if (Visited.Num() == TotalRooms) return true;

	for (ERoomType Neighbor : Graph[Current])
	{
		if (Excluded.Contains(Neighbor) || Visited.Contains(Neighbor))
			continue;

		bool IsPassageThroughRSM = (Neighbor == ERoomType::RSM);
		if (IsPassageThroughRSM && Current != ERoomType::RSM)
		{
			Path.Add(Neighbor);
			if (FindValidRound(Neighbor, Path, Visited, Excluded)) return true;
			Path.Pop();
			continue;
		}
		Visited.Add(Neighbor);
		Path.Add(Neighbor);

		if (FindValidRound(Neighbor, Path, Visited, Excluded)) return true;

		Visited.Remove(Neighbor);
		Path.Pop();
	}

	return false;
}


TArray<ERoomType> AHearingNeighbourController::GetValidRound(TSet<ERoomType> Excluded)
{
	TArray Path = {ERoomType::RSM};
	TSet<ERoomType> Visited;

	if (FindValidRound(CurrentRoom, Path, Visited, Excluded))
	{
		return Path;
	}

	return {};
}

void AHearingNeighbourController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	APrisoner* Player = Cast<APrisoner>(Actor);

	if (!SightConfig) return;

	if (Stimulus.Type == SightConfig->GetSenseID() && Player)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			if (Player->IsAlive())
				PlayersInSight.AddUnique(Player);
			if (PlayerTarget && CurrentTimerBeforeCanSwitchToTarget > 0.0f) return;
			if (PlayerTarget && PlayerTarget != Player)
			{
				float DistanceToNewPlayer =
					FVector::Distance(Player->GetActorLocation(), GetPawn()->GetActorLocation());
				float DistanceToCurrentPlayer = FVector::Distance(PlayerTarget->GetActorLocation(),
				                                                  GetPawn()->GetActorLocation());
				if (DistanceToNewPlayer > DistanceToCurrentPlayer) return;
			}
			if (!Player->IsAlive()) return;
			//SetFocus(Actor);

			if (PlayerTarget != Player)
			{
				AAHearingNeighbourCharacter* NeighbourCharacter = Cast<AAHearingNeighbourCharacter>(GetPawn());
				if (NeighbourCharacter)
				{
					NeighbourCharacter->OnAlert(Player);
					NeighbourCharacter->OnLostSighPlayer(PlayerTarget);
				}
			}

			PlayerTarget = Player;
			CurrentTimerBeforeCanSwitchToTarget = NeighbourEntityData->TimerBeforeCanSwitchToTarget;
			if (!Player || !PlayerTarget) return;
			Player->OnCharacterDied.RemoveDynamic(this, &AHearingNeighbourController::ClearTarget);
			Player->OnCharacterDied.AddDynamic(this, &AHearingNeighbourController::ClearTarget);
			return;
		}
		if (Player == PlayerTarget)
		{
			ClearTarget(-1);
		}
		PlayersInSight.Remove(Player);
		return;
	}
	
	if (Stimulus.Type == HearingConfig->GetSenseID() && Stimulus.WasSuccessfullySensed())
	{
		if (PlayerTarget && PlayerCantBeHeard.Contains(PlayerTarget)) return;
		LastHearingSoundStrength = CalcStrengthFromDistanceAndStrength(
			FVector::Distance(Stimulus.StimulusLocation, GetPawn()->GetActorLocation()), Stimulus.Strength);
		LastHearingSoundActor = Actor;
		return;
	}
}

void AHearingNeighbourController::SwitchToNewTarget()
{
	if (!PlayerTarget && PlayersInSight.Num() > 0)
	{
		PlayerTarget = PlayersInSight[0];
		//SetFocus(PlayerTarget);
		return;
	}

	APrisoner *OtherNearbyPlayer = nullptr;
	
	if (PlayerTarget && PlayersInSight.Num() > 0 && CurrentTimerBeforeCanSwitchToTarget <= 0.0f)
	{
		for (auto PrisonerInSight : PlayersInSight)
		{
			if (PrisonerInSight == PlayerTarget) continue;

			float DistanceToNewPlayer =
				FVector::Distance(PrisonerInSight->GetActorLocation(), GetPawn()->GetActorLocation());
			float DistanceToCurrentPlayer = FVector::Distance(PlayerTarget->GetActorLocation(),
			                                                  GetPawn()->GetActorLocation());
			if (DistanceToNewPlayer > DistanceToCurrentPlayer) continue;
			if (!PrisonerInSight->IsAlive()) continue;
			OtherNearbyPlayer = PrisonerInSight;
		}
	}
	
	if (OtherNearbyPlayer)
	{
		//SetFocus(OtherNearbyPlayer);

		AAHearingNeighbourCharacter* NeighbourCharacter = Cast<AAHearingNeighbourCharacter>(GetPawn());
		if (NeighbourCharacter)
		{
			NeighbourCharacter->OnAlert(OtherNearbyPlayer);
			NeighbourCharacter->OnLostSighPlayer(PlayerTarget);
		}

		PlayerTarget = OtherNearbyPlayer;
		CurrentTimerBeforeCanSwitchToTarget = NeighbourEntityData->TimerBeforeCanSwitchToTarget;
		if (!OtherNearbyPlayer || !PlayerTarget) return;
		OtherNearbyPlayer->OnCharacterDied.RemoveDynamic(this, &AHearingNeighbourController::ClearTarget);
		OtherNearbyPlayer->OnCharacterDied.AddDynamic(this, &AHearingNeighbourController::ClearTarget);
	}
}

void UpdateModifier(float& CurrentModifier, float TargetModifier, float RatioModifierPerSecond, float DeltaSeconds)
{
	if (CurrentModifier < TargetModifier)
	{
		CurrentModifier += RatioModifierPerSecond * DeltaSeconds;
		if (CurrentModifier > TargetModifier)
			CurrentModifier = TargetModifier;
	}
	else if (CurrentModifier > TargetModifier)
	{
		CurrentModifier -= RatioModifierPerSecond * DeltaSeconds;
		if (CurrentModifier < TargetModifier)
			CurrentModifier = TargetModifier;
	}
}

void AHearingNeighbourController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (CurrentState)
	{
	case EEntityState::Patrol:
		UpdateModifier(CurrentModifierSight, NeighbourEntityData->ModifierSightOfPatrol,
		               NeighbourEntityData->RatioModifierSightOfPatrolPerSecond, DeltaSeconds);
		UpdateModifier(CurrentModifierHearing, NeighbourEntityData->ModifierHeardOfPatrol,
		               NeighbourEntityData->RatioModifierHearingOfPatrolPerSecond, DeltaSeconds);
		break;
	case EEntityState::Chase:
		UpdateModifier(CurrentModifierSight, NeighbourEntityData->ModifierSightOfChase,
		               NeighbourEntityData->RatioModifierSightOfChasePerSecond, DeltaSeconds);
		UpdateModifier(CurrentModifierHearing, NeighbourEntityData->ModifierHeardOfChase,
		               NeighbourEntityData->RatioModifierHearingOfChasePerSecond, DeltaSeconds);
		break;
	case EEntityState::Hunt:
		UpdateModifier(CurrentModifierSight, NeighbourEntityData->ModifierSightOfHunt,
		               NeighbourEntityData->RatioModifierSightOfHuntPerSecond, DeltaSeconds);
		UpdateModifier(CurrentModifierHearing, NeighbourEntityData->ModifierHeardOfHunt,
		               NeighbourEntityData->RatioModifierHearingOfHuntPerSecond, DeltaSeconds);
		break;
	}

	if (HearingConfig)
	{
		HearingConfig->HearingRange = NeighbourEntityData->HearingRange + CurrentModifierSight;
	}
	if (SightConfig)
	{
		SightConfig->SightRadius = NeighbourEntityData->SightRadius + CurrentModifierHearing;
	}

	if (UltimateSprintTask.IsValid())
	{
		UltimateSprintTask->Cooldown -= DeltaSeconds;
		if (UltimateSprintTask->Cooldown <= 0.0f)
		{
			UltimateSprintTask.Reset();
		}
	}
	if (CurrentTimerBeforeCanSwitchToTarget > 0.0f)
	{
		CurrentTimerBeforeCanSwitchToTarget -= DeltaSeconds;
	}
	else
	{
		SwitchToNewTarget();
	}
}
