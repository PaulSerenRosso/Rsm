// Fill out your copyright notice in the Description page of Project Settings.


#include "FindBiggestSoundLocationTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "ProjectEscapeProto/DataAssets/HearingNeighbourData.h"
#include "ProjectEscapeProto/IA/AICharacter.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"

UFindBiggestSoundLocationTask::UFindBiggestSoundLocationTask()
{
	NodeName = "Find Sound Location";
}

EBTNodeResult::Type UFindBiggestSoundLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AHearingNeighbourController* AIController = Cast<AHearingNeighbourController>(OwnerComp.GetAIOwner());

	if (!AIController) return EBTNodeResult::Failed;
	if (!AIController->LastHearingSoundActor) return EBTNodeResult::Succeeded;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	UAIPerceptionComponent* PerceptionComponent = AIController->FindComponentByClass<UAIPerceptionComponent>();
	if (!PerceptionComponent) return EBTNodeResult::Failed;

	FVector BestSoundLocation = FVector::ZeroVector;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	if (AIController->LastHearingSoundStrength > 0.0f)
	{
		float DistanceTo = NeighbourEntityData->HearingRange;
		DistanceTo -= ControlledPawn->GetDistanceTo(AIController->LastHearingSoundActor);
		const float StrengthRatio = AIController->LastHearingSoundStrength + ((DistanceTo > 0) ? DistanceTo : 0.0f);

		if (StrengthRatio > BestSoundStrength)
		{
			BestSoundStrength = StrengthRatio;
			BlackboardComp->SetValueAsObject(SoundActorKey.SelectedKeyName, AIController->LastHearingSoundActor);
			AAICharacter* AICharacter = Cast<AAICharacter>(ControlledPawn);
			if (AICharacter)
			AICharacter->OnHearSound();
		}
		AIController->LastHearingSoundStrength = 0.0f;
		AActor* LastHearingSoundActor = Cast<AActor>(BlackboardComp->GetValueAsObject(SoundActorKey.SelectedKeyName));
		float LastHearingSoundStrength = (BestSoundStrength <= 0.0f) ? 0.0f : BestSoundStrength;
		BestSoundLocation = (LastHearingSoundActor) ? LastHearingSoundActor->GetActorLocation() : BestSoundLocation;

		if (BestSoundLocation != FVector::ZeroVector)
		{
			if (!NavSystem.IsValid())
				NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			FNavLocation NavLocation;
			if (NavSystem->ProjectPointToNavigation(BestSoundLocation, NavLocation))
				BlackboardComp->SetValueAsVector(SoundLocationKey.SelectedKeyName, NavLocation.Location);
		}
		AIController->SwapState(EEntityState::Chase);
		BlackboardComp->SetValueAsFloat(LastHearingSoundStrengthKey.SelectedKeyName, LastHearingSoundStrength);
	}
	else
	{
		BestSoundStrength = 0.0f;
	}

	return EBTNodeResult::Succeeded;
}
