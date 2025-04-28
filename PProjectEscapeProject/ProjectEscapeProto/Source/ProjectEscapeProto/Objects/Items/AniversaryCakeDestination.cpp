// Fill out your copyright notice in the Description page of Project Settings.


#include "AniversaryCakeDestination.h"

#include "AnniversaryCake.h"
#include "Components/BoxComponent.h"

// Sets default values
AAniversaryCakeDestination::AAniversaryCakeDestination()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
}

// Called when the game starts or when spawned
void AAniversaryCakeDestination::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAniversaryCakeDestination::OnCakeArrived);
}

void AAniversaryCakeDestination::OnCakeArrived(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	AAnniversaryCake* Cake = Cast<AAnniversaryCake>(OtherActor);
	if (!Cake) return;
	Cake->CakeArrived();
}

// Called every frame
void AAniversaryCakeDestination::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

