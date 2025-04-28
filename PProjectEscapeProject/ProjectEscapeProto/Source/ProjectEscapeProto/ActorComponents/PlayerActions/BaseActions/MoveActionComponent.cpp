// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveActionComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/TweakeableValueCharacterData.h"
#include "ProjectEscapeProto/Utilities/EnumsUtilities.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"
#include "ProjectEscapeProto/Utilities/MyBitReader.h"
#include "ProjectEscapeProto/Utilities/MyBitWriter.h"


// Sets default values for this component's properties
UMoveActionComponent::UMoveActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMoveActionComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UMoveActionComponent::AskExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if(!CanExecuteAction()) return;
	MyBitReader->Init(Payload);
	FVector2D Direction = MyBitReader->GetNextVector2DValue();
	float NewWalkSpeed = 0.0f;
	float NewCrouchWalkSpeed = MyPrisoner->TweakeableData->CrouchedMoveSpeed;

	if (!UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask, StaticCast<int32>(EPlayerActionState::IsSprinting))
		&& !UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask,
		                                              StaticCast<int32>(EPlayerActionState::IsWalkingQuiet)) &&
		!UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask, StaticCast<int32>(EPlayerActionState::IsCrouching)))
	{
	
		NewWalkSpeed = (Direction.Y > 0)
			               ?MyPrisoner->TweakeableData->BaseForwardMoveSpeed
			               : MyPrisoner->TweakeableData->BaseBackwardMoveSpeed;

		if (abs(Direction.X) > 0.1 && Direction.Y >= 0)
			NewWalkSpeed = MyPrisoner->TweakeableData->BaseSideMoveSpeed;

	}
	else if (UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask,
	                                                   StaticCast<int32>(EPlayerActionState::IsSprinting)))
	{
		
		NewWalkSpeed = (abs(Direction.X) > 0.1)
			               ? (Direction.Y >= 0)
				                 ? MyPrisoner->TweakeableData->SprintSideMoveSpeed
				                 : MyPrisoner->TweakeableData->SprintBackwardMoveSpeed
			               : (Direction.Y >= 0)
			               ? MyPrisoner->TweakeableData->SprintForwardMoveSpeed
			               : MyPrisoner->TweakeableData->SprintBackwardMoveSpeed;

	}
	else if (UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask,
	                                                   StaticCast<int32>(EPlayerActionState::IsWalkingQuiet)))
	{
	
		NewWalkSpeed = (Direction.Y >= 0)
			               ? MyPrisoner->TweakeableData->WalkForwardMoveSpeed
			               : MyPrisoner->TweakeableData->WalkBackwardMoveSpeed;
	}
	
	
	NewWalkSpeed *= GetSlowRatio();
	NewCrouchWalkSpeed *= GetSlowRatio();
	
	if (LocalCrouchWalkSpeed!= NewCrouchWalkSpeed)
	{
		LocalCrouchWalkSpeed = NewCrouchWalkSpeed;
		AssignMaxCrouchWalkSpeed(LocalCrouchWalkSpeed);
		
	}
	
	if (LocalWalkSpeed != NewWalkSpeed)
	{
		LocalWalkSpeed = NewWalkSpeed;
		MyPrisoner->GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
		AssignMaxWalkSpeed(LocalWalkSpeed);
	}
	

	FRotator MyControlRotation = MyPrisoner->GetControlRotation();
	MyControlRotation.Pitch = 0.0f;
	MyControlRotation.Roll = 0.0f;

	FVector ForwardDirection = FRotationMatrix(MyControlRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(MyControlRotation).GetUnitAxis(EAxis::Y);
	
	MyPrisoner->AddMovementInput(RightDirection, Direction.X);
	MyPrisoner->AddMovementInput(ForwardDirection, Direction.Y);
	
	Super::AskExecuteAction(Payload, Actors);
}


void UMoveActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	if(!CanExecuteAction()) return;
	MyBitReader->Init(Payload);
	FVector2D Direction = MyBitReader->GetNextVector2DValue();
	float NewWalkSpeed = 0.0f;
	float NewCrouchWalkSpeed = MyPrisoner->TweakeableData->CrouchedMoveSpeed;

	if (!UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask, StaticCast<int32>(EPlayerActionState::IsSprinting))
		&& !UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask,
		                                              StaticCast<int32>(EPlayerActionState::IsWalkingQuiet)) &&
		!UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask, StaticCast<int32>(EPlayerActionState::IsCrouching)))
	{
	
		NewWalkSpeed = (Direction.Y > 0)
			               ?MyPrisoner->TweakeableData->BaseForwardMoveSpeed
			               : MyPrisoner->TweakeableData->BaseBackwardMoveSpeed;

		if (abs(Direction.X) > 0.1 && Direction.Y >= 0)
			NewWalkSpeed = MyPrisoner->TweakeableData->BaseSideMoveSpeed;

		MyPrisoner->GenerateSound(MyPrisoner->TweakeableData->BaseNoiseEmitted);
	}
	else if (UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask,
	                                                   StaticCast<int32>(EPlayerActionState::IsSprinting)))
	{
		
		NewWalkSpeed = (abs(Direction.X) > 0.1)
			               ? (Direction.Y >= 0)
				                 ? MyPrisoner->TweakeableData->SprintSideMoveSpeed
				                 : MyPrisoner->TweakeableData->SprintBackwardMoveSpeed
			               : (Direction.Y >= 0)
			               ? MyPrisoner->TweakeableData->SprintForwardMoveSpeed
			               : MyPrisoner->TweakeableData->SprintBackwardMoveSpeed;

		MyPrisoner->GenerateSound(MyPrisoner->TweakeableData->SprintNoiseEmitted);
	}
	else if (UFunctionsUtilities::BinaryMask_HaveValue(*PlayerActionStateMask,
	                                                   StaticCast<int32>(EPlayerActionState::IsWalkingQuiet)))
	{
	
		NewWalkSpeed = (Direction.Y >= 0)
			               ? MyPrisoner->TweakeableData->WalkForwardMoveSpeed
			               : MyPrisoner->TweakeableData->WalkBackwardMoveSpeed;
		MyPrisoner->GenerateSound(MyPrisoner->TweakeableData->WalkNoiseEmitted);
	}
	
	
	NewWalkSpeed *= GetSlowRatio();
	NewCrouchWalkSpeed *= GetSlowRatio();
	
	if (LocalCrouchWalkSpeed!= NewCrouchWalkSpeed)
	{
		LocalCrouchWalkSpeed = NewCrouchWalkSpeed;
		AssignMaxCrouchWalkSpeed(LocalCrouchWalkSpeed);
		
	}
	
	if (LocalWalkSpeed != NewWalkSpeed)
	{
		LocalWalkSpeed = NewWalkSpeed;
		MyPrisoner->GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
		AssignMaxWalkSpeed(LocalWalkSpeed);
	}
	

	// FRotator MyControlRotation = MyPrisoner->GetControlRotation();
	// MyControlRotation.Pitch = 0.0f;
	// MyControlRotation.Roll = 0.0f;

	// FVector ForwardDirection = FRotationMatrix(MyControlRotation).GetUnitAxis(EAxis::X);
	// FVector RightDirection = FRotationMatrix(MyControlRotation).GetUnitAxis(EAxis::Y);
	//
	// MyPrisoner->AddMovementInput(RightDirection, Direction.X);
	// MyPrisoner->AddMovementInput(ForwardDirection, Direction.Y);
	
	MyBitWriter->Init();
	MyBitWriter->SetVector2D(Direction);
	MyBitWriter->SetFloat(LocalWalkSpeed);
	MyBitWriter->SetFloat(LocalCrouchWalkSpeed);

	Super::MyServer_ExecuteAction(MyBitWriter->GetData(), Actors);
	Multicast_ExecuteAction(MyBitWriter->GetData(), Actors);

}



void UMoveActionComponent::MyMulticast_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyMulticast_ExecuteAction(Payload, Actors);
}

void UMoveActionComponent::AssignMaxWalkSpeed(float Value)
{
	UCharacterMovementComponent* Movement = MyPrisoner->GetCharacterMovement();
	if (!Movement) return;
	Movement->MaxWalkSpeed = Value;
}

void UMoveActionComponent::AssignMaxCrouchWalkSpeed(float Value)
{
	UCharacterMovementComponent* Movement = MyPrisoner->GetCharacterMovement();
	if (!Movement) return;
	Movement->MaxWalkSpeedCrouched = Value;
}

void UMoveActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMoveActionComponent::InitMulticastAction(APrisoner* InPrisoner)
{
	Super::InitMulticastAction(InPrisoner);
	TweakableData = MyPrisoner->TweakeableData;
	
	PlayerActionStateMask = &MyPrisoner->PlayerActionStateMask;
}

void UMoveActionComponent::InitServerAction(APrisoner* InPrisoner)
{
	Super::InitServerAction(InPrisoner);
}

void UMoveActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMoveActionComponent, CurrentSlowRatio);
}




