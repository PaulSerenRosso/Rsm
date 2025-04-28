// Fill out your copyright notice in the Description page of Project Settings.


#include "LookActionComponent.h"

#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/TweakeableValueCharacterData.h"
#include "ProjectEscapeProto/Utilities/MyBitReader.h"
#include "ProjectEscapeProto/Utilities/MyBitWriter.h"


// Sets default values for this component's properties
ULookActionComponent::ULookActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULookActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void ULookActionComponent::MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyMulticast_ExecuteAction(Payload, Actors);
	MyBitReader->Init(Payload);
	FVector2D RotationVector = MyBitReader->GetNextVector2DValue();
	MyPrisoner->AddControllerYawInput(RotationVector.X);
	
	CameraComponent->SetWorldRotation(FRotator(RotationVector.Y,
										   CameraComponent->GetComponentRotation().Yaw,
										   CameraComponent->GetComponentRotation().Roll));
}

void ULookActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if(!CanExecuteAction()) return;
	MyBitReader->Init(Payload);
	FVector2D RotationVector = MyBitReader->GetNextVector2DValue();
	float NewPitch = -1 * RotationVector.Y * TweakeableValueCharacterData->CameraRotationPitchSpeed + CameraComponent->
																		  GetComponentRotation().Pitch;
	NewPitch = FMath::Clamp(NewPitch, TweakeableValueCharacterData->CameraRotationPitchMin, TweakeableValueCharacterData->CameraRotationPitchMax);
	CameraComponent->SetWorldRotation(FRotator(NewPitch, CameraComponent->GetComponentRotation().Yaw,
											   CameraComponent->GetComponentRotation().Roll));

	MyBitWriter->Init();
	MyBitWriter->SetVector2D(FVector2D(RotationVector.X,NewPitch));
	
	Super::MyServer_ExecuteAction(MyBitWriter->GetData(), Actors);
	Multicast_ExecuteAction(MyBitWriter->GetData(), Actors);
}

void ULookActionComponent::InitMulticastAction(APrisoner* InPrisoner)
{
	Super::InitMulticastAction(InPrisoner);
	CameraComponent = InPrisoner->GetCameraComponent();
	TweakeableValueCharacterData = InPrisoner->TweakeableData;
}






	
	