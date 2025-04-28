// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrench.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/TweakValueWrenchData.h"
#include "ProjectEscapeProto/Interfaces/Hitable.h"

AWrench::AWrench()
{
	CurrentTimeToBeReusable = 0;
}

void AWrench::BeginPlay()
{
	Super::BeginPlay();
	Durability = WrenchData->Durability;
	if (!WrenchData) return;
	TimeToBeReusable = WrenchData->TimeToBeReusable;
}

void AWrench::UseItem()
{
	if (!CanBeReused()) return;
	Super::UseItem();

	CurrentTimeToBeReusable = TimeToBeReusable;
	APrisoner* ObjectOwnerPlayer = Cast<APrisoner>(GetOwner());

	if (!ObjectOwnerPlayer && !ObjectOwner) return;
	
	FVector ForwardVector = (ObjectOwnerPlayer)
		                        ? ObjectOwnerPlayer->GetCameraForwardVector()
		                        : ObjectOwner->GetActorForwardVector();
	FVector StartLocation = (ObjectOwnerPlayer)
		                        ? ObjectOwnerPlayer->GetCameraPosition()
		                        : ObjectOwner->GetActorLocation();
	
	StartLocation += ForwardVector * 100.0f; // Offset
	FVector EndLocation = StartLocation + (ForwardVector * 500.0f);

	float SphereRadius = 50.0f;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereRadius);
	FCollisionQueryParams SweepParams;
	SweepParams.AddIgnoredActor(this);
	SweepParams.AddIgnoredActor(ObjectOwner);
	FHitResult HitResult;

	if (!GetWorld()->SweepSingleByChannel(HitResult,
	                                      StartLocation,
	                                      EndLocation,
	                                      FQuat::Identity,
	                                      ECC_Camera,
	                                      CollisionShape,
	                                      SweepParams))
		return;

	//DrawDebugSphere(GetWorld(), HitResult.Location, SphereRadius, 12, FColor::Red, false, 5.0f);

	IIHitable* Hitable = Cast<IIHitable>(HitResult.GetActor());
	if (!Hitable) return;

	if (!Hitable->CanHit(this)) return;
	Durability--;
	Hitable->TryHit(ObjectOwner);
	if (Durability == 0)
	{
		ConsumeItem();
		Destroy();
		return;
	};
}
