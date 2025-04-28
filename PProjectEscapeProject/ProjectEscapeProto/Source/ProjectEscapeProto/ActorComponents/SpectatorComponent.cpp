// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorComponent.h"

#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/Characters/PrisonerController.h"

// Sets default values for this component's properties
USpectatorComponent::USpectatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpectatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpectatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USpectatorComponent::SwitchCamera_Implementation(bool Next, APrisonerController* MyController)
{
	if (!MyController) return;
	AOnlineGameMode* InGameGameMode = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	if (!InGameGameMode) return;
	APrisonerController* CurrentSpec = CamSpectatorPawn ? CamSpectatorPawn : MyController;
	TWeakObjectPtr<APrisonerController> NextPrisoner = InGameGameMode->GetNextOrPrevAlivePC(CurrentSpec, Next);
	if (!NextPrisoner.IsValid()) return;
	APrisoner* NextPrisonerPawn = Cast<APrisoner>(NextPrisoner->GetPawn());
	if (!NextPrisonerPawn) return;
	CamSpectatorPawn = NextPrisoner.Get();
	MyController->ChangeCam(NextPrisonerPawn);
}

bool USpectatorComponent::SwitchCamera_Validate(bool Next, APrisonerController* MyController)
{
	return true;
}

