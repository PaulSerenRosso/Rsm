// Fill out your copyright notice in the Description page of Project Settings.


#include "GetRandomPointOnRadiusTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UGetRandomPointOnRadiusTask::UGetRandomPointOnRadiusTask(): Radius(500)
{
	NodeName = "Get Random Point On Radius";
}

EBTNodeResult::Type UGetRandomPointOnRadiusTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	APawn* Pawn = AIController->GetPawn();
	FVector Origin = BlackboardComp->GetValueAsVector(LocationSourceKey.SelectedKeyName);
	//FVector Origin = AIController->GetPawn()->GetActorLocation();


	FVector RandomPoint;
	for (int i = 0; i < 100; i++)
	{
		if (GetValidRandomPoint(RandomPoint, Origin, Pawn))
		{
			BlackboardComp->SetValueAsVector(RandomPointKey.SelectedKeyName, RandomPoint);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

bool UGetRandomPointOnRadiusTask::GetValidRandomPoint(FVector& OutLocation, const FVector& Origin, AActor* Owner) const
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (!NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation RandomLocation;
	if (NavSystem->GetRandomReachablePointInRadius(Origin, Radius, RandomLocation))
	{
		OutLocation = RandomLocation.Location;

		FHitResult HitResult;
		FVector TraceStart = Origin;
		FVector TraceEnd = OutLocation;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Owner);

		UE_LOG(LogTemp, Warning, TEXT("OutLocation: %s"), *OutLocation.ToString());

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

		if (!bHit)
		{
			DrawDebugSphere(GetWorld(), OutLocation, 20.0f, 12, FColor::Green, false, 2.0f);
			return true;
		}
	}
	return false;
}
