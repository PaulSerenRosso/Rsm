// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

UMoveToLocationTask::UMoveToLocationTask()
{
	NodeName = "Move To Location Task";
}

EBTNodeResult::Type UMoveToLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	FVector Location = BlackboardComp->GetValueAsVector(LocationKey.SelectedKeyName);
	FVector CurrentPawnLocation = AIController->GetPawn()->GetActorLocation();
	CurrentPawnLocation.Z -= AIController->GetPawn()->GetSimpleCollisionHalfHeight();
	float Distance = FVector::Distance(CurrentPawnLocation, Location);
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("Distance: %f"), Distance));
	EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(Location); 
	if (Result != EPathFollowingRequestResult::Type::Failed && Distance <= AcceptanceRadius)
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}