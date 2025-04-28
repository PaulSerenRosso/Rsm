// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreDrop.h"

#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"

// Sets default values
AScoreDrop::AScoreDrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AScoreDrop::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerBeforeActivation;
	GetWorld()->GetTimerManager().SetTimer(TimerBeforeActivation, this, &AScoreDrop::Activate, 0.2f, false);
}

void AScoreDrop::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APrisoner* Prisoner = Cast<APrisoner>(OtherActor);
	if (!Prisoner) return;
	if (Prisoner->IsLocallyControlled())
		Prisoner->UpdateScore(Score);
	if (HasAuthority())
		Destroy();
}

void AScoreDrop::Activate()
{
	Mesh->SetVisibility(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionProfileName(TEXT("OverlapAll"));
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AScoreDrop::OnOverlapBegin);
}


void AScoreDrop::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AScoreDrop, Score);
}

// Called every frame
void AScoreDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
