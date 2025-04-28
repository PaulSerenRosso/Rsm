// Fill out your copyright notice in the Description page of Project Settings.


#include "AnniversaryCake.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/Characters/PrisonerController.h"
#include "ProjectEscapeProto/DataAssets/AnniversaryCakeData.h"

AAnniversaryCake::AAnniversaryCake()
{
	bReplicates = true;
	FirstPersonPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FirstPersonPosition"));
	SecondPersonPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SecondPersonPosition"));

	FirstPersonPosition->SetupAttachment(ItemMesh);
	SecondPersonPosition->SetupAttachment(ItemMesh);
}

void AAnniversaryCake::CakeArrived()
{
	if (!HasAuthority()) return;
	AOnlineGameMode* GM = GetWorld()->GetAuthGameMode<AOnlineGameMode>();
	if (!GM) return;
	if (FirstPlayer.IsValid())
	{
		GM->AskUpdateScore(CakeData->ScoreWhenArrived, FirstPlayer->GetPlayerState()->GetPlayerId());
		OnFirstPlayerDied(FirstPlayer->GetPlayerState()->GetPlayerId());
	}
	if (SecondPlayer.IsValid())
	{
		GM->AskUpdateScore(CakeData->ScoreWhenArrived, SecondPlayer->GetPlayerState()->GetPlayerId());
		OnSecondPlayerDied(SecondPlayer->GetPlayerState()->GetPlayerId());
	}
	OnCakeArrived.Broadcast();
	Destroy();
}

void AAnniversaryCake::MoveToPos(FVector NewLocation)
{
	SetActorLocation(NewLocation);
	Multicast_MoveToPos(NewLocation);
}

void AAnniversaryCake::Multicast_MoveToPos_Implementation(FVector NewLocation)
{
	SetActorLocation(NewLocation);
}

void AAnniversaryCake::BeginPlay()
{
	Super::BeginPlay();
}

void AAnniversaryCake::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TryToMoveCake(DeltaSeconds);
}

void AAnniversaryCake::TryToMoveCake(float DeltaSeconds)
{
	if (!HasAuthority()  || !CanBeUsed) return;
	if (!FirstPlayer.IsValid() || !SecondPlayer.IsValid()) return;
	FVector FirstVelocity = FirstPlayer->GetCharacterMovement()->Velocity;
	FVector SecondVelocity = SecondPlayer->GetCharacterMovement()->Velocity;

	FVector MoveDir1 = FirstVelocity.GetSafeNormal();
	FVector MoveDir2 = SecondVelocity.GetSafeNormal();

	if (MoveDir1.IsNearlyZero() && MoveDir2.IsNearlyZero()) return;

	FVector Direction = (MoveDir1 + MoveDir2).GetSafeNormal();

	FVector NewLocation = GetActorLocation() + Direction * CakeData->MoveSpeed * DeltaSeconds;
	MoveToPos(NewLocation);
}

void AAnniversaryCake::OnFirstPlayerDied(int32 PlayerId)
{
	if (FirstPlayer.IsValid())
	{
		FirstPlayer->OnCharacterDied.RemoveDynamic(this, &AAnniversaryCake::OnFirstPlayerDied);
		FirstPlayer = nullptr;
	}
}

void AAnniversaryCake::OnSecondPlayerDied(int32 PlayerId)
{
	if (SecondPlayer.IsValid())
	{
		SecondPlayer->OnCharacterDied.RemoveDynamic(this, &AAnniversaryCake::OnSecondPlayerDied);
		SecondPlayer = nullptr;
	}
}


bool AAnniversaryCake::TryInteract_Implementation(AActor* Actor)
{
	APrisoner* Prisoner = Cast<APrisoner>(Actor);
	if (!Prisoner  || !CanBeUsed) return false;
	OnBeginToInteract();
	if (FirstPlayer == Prisoner)
	{
		OnFirstPlayerDied(FirstPlayer->GetPlayerState()->GetPlayerId());
	}
	else if (SecondPlayer == Prisoner)
	{
		OnSecondPlayerDied(SecondPlayer->GetPlayerState()->GetPlayerId());
	}
	else if (!FirstPlayer.IsValid())
	{
		FirstPlayer = Prisoner;
		Prisoner->SetActorLocation(FirstPersonPosition->GetComponentLocation());
		FirstPlayer->OnCharacterDied.AddDynamic(this, &AAnniversaryCake::OnFirstPlayerDied);
	}
	else if (!SecondPlayer.IsValid())
	{
		SecondPlayer = Prisoner;
		Prisoner->SetActorLocation(SecondPersonPosition->GetComponentLocation());
		SecondPlayer->OnCharacterDied.AddDynamic(this, &AAnniversaryCake::OnSecondPlayerDied);
	}

	return Super::TryInteract_Implementation(Actor);
}

bool AAnniversaryCake::TryStopInteract_Implementation(AActor* Actor)
{
	return Super::TryStopInteract_Implementation(Actor);
}
