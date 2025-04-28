// Fill out your copyright notice in the Description page of Project Settings.


#include "IsAtLocationPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UIsAtLocationPoint::UIsAtLocationPoint()
{
	
}

bool UIsAtLocationPoint::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	
	FVector Location = BlackboardComp->GetValueAsVector(LocationKey.SelectedKeyName);
	
	FVector CurrentPawnLocation = AIController->GetPawn()->GetActorLocation();
	CurrentPawnLocation.Z -= AIController->GetPawn()->GetSimpleCollisionHalfHeight();
	
	return (FVector::Distance(CurrentPawnLocation, Location) <= AcceptanceRadius);
}
