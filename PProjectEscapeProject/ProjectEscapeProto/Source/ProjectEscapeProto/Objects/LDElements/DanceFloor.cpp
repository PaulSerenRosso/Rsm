// Fill out your copyright notice in the Description page of Project Settings.


#include "DanceFloor.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/DancefloorData.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"

ADanceFloor::ADanceFloor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(RootComponent);
	
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(ItemMesh);
	FloorMesh->OnComponentBeginOverlap.AddDynamic(this, &ADanceFloor::OnOverlapBegin);
	FloorMesh->OnComponentEndOverlap.AddDynamic(this, &ADanceFloor::OnOverlapEnd);
}

void ADanceFloor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!HasAuthority())
	{
		if (TextRender->Text.ToString() != CurrentTimerText)
			TextRender->SetText(FText::FromString(CurrentTimerText));
		return;
	}

	if (TimerBeforeFinish > 0 && DanseStarted == '1')
	{
		TimerBeforeFinish -= DeltaSeconds;
		CurrentTimerText = FString::Printf(TEXT("%.3f"), TimerBeforeFinish);
		TextRender->SetText(FText::FromString(CurrentTimerText));

		for (auto PlayerID : PlayerInteracting)
		{
			PlayerInteractingTimer[PlayerID] -= DeltaSeconds;
			if (PlayerInteractingTimer[PlayerID] <= 0)
			{
				PlayerInteractingTimer[PlayerID] = DancefloorData->DelayTickBetweenPoints;
				TickReward(PlayerID);
			}
		}

		UFunctionsUtilities::Server_MakeNoise(this, this, GetActorLocation(),
									  DancefloorData->LoudnessNoiseEmitted);
		
		if (TimerBeforeFinish <= 0 || !IncompleteInteraction)
		{
			TextRender->SetText(FText::FromString(FString::Printf(TEXT("FINISHED"))));
			CurrentTimerText = "FINISHED";
			DanseStarted = '2';
		IncompleteInteraction = false;
			if (PlayerInteracting.Num() > 0)
				for (auto PlayerId : PlayerInteracting)
				{
					int IndexOf = PlayerInteracting.Find(PlayerId);
					if (IndexOf == INDEX_NONE) continue;
					PlayerInteracting.RemoveAt(IndexOf);
					// FActionTimer* Timer = PlayerInteractingTimer[IndexOf];
					// AActionTimerManager::RemoveActionTimer(Timer->OnActionTimerManagerTick);
					PlayerInteractingTimer.Remove(PlayerId);
				}
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				OnDancefloorFinished.Broadcast();
			}, 0.001f, false);
			//OnDancefloorFinished.Broadcast();
		}

		
		// UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(),
		//                                    DancefloorData->LoudnessNoiseEmitted / MAX_NOISE, this,
		//                                    DancefloorData->MaxRangeNoiseEmitted, NAME_None);
	}
}

void ADanceFloor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	//if (!HasAuthority() || !IncompleteInteraction) return;
	if (!HasAuthority() && StillInteractable) return;
	APrisoner* Prisoner = Cast<APrisoner>(OtherActor);
	if (!Prisoner) return;

	PlayerInteracting.Add(Prisoner->GetPlayerState()->GetPlayerId());

	if (PlayerInteracting.Num() >= DancefloorData->NbPlayersNeedToStart && DanseStarted == '0')
	{
		DanseStarted = '1';
		TimerBeforeFinish = DancefloorData->TimeBeforeFinish;
		
		OnBeginToInteract();
		OnDancefloorStarted.Broadcast();
	}
	
	PlayerInteractingTimer.Add(Prisoner->GetPlayerState()->GetPlayerId(), DancefloorData->DelayTickBetweenPoints);
}

void ADanceFloor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;
	APrisoner* Prisoner = Cast<APrisoner>(OtherActor);
	if (Prisoner)
	{
		//PlayerInteracting.Remove(Prisoner->GetPlayerState()->GetPlayerId());
		int IndexOf = PlayerInteracting.Find(Prisoner->GetPlayerState()->GetPlayerId());
		if (IndexOf == INDEX_NONE) return;
		PlayerInteracting.RemoveAt(IndexOf);
		PlayerInteractingTimer.Remove(Prisoner->GetPlayerState()->GetPlayerId());
	}
}

void ADanceFloor::TickReward(int Id)
{
	if (!HasAuthority() || DanseStarted != '1' || !DancefloorData || Id == -1) return;
	if (!GM.IsValid())
		GM = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM.IsValid()) return;
	GM->AskUpdateScore(DancefloorData->PointByTick, Id);
}

void ADanceFloor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADanceFloor, CurrentTimerText);
}
