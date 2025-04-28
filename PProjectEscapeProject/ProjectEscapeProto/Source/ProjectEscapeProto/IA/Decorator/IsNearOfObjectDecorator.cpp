// Fill out your copyright notice in the Description page of Project Settings.


#include "IsNearOfObjectDecorator.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UIsNearOfObjectDecorator::UIsNearOfObjectDecorator()
{
	NodeName = TEXT("Is Near Of Object");
	AcceptanceRadius = 100;
}

bool UIsNearOfObjectDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
	{
		return false;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	AActor* ObjectToCheck = Cast<AActor>(BlackboardComp->GetValueAsObject(ObjectToCheckKey.SelectedKeyName));
	if (!ObjectToCheck)
	{
		return false;
	}

	float Distance = FVector::Dist(AIPawn->GetActorLocation(), ObjectToCheck->GetActorLocation());
	return Distance <= AcceptanceRadius;
}
