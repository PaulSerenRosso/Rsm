// Fill out your copyright notice in the Description page of Project Settings.


#include "PushActionComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectEscapeProto/ActorComponents/StaminaSystem.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/PushData.h"
#include "ProjectEscapeProto/Interfaces/Pushable.h"

void UPushActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentCooldown > 0.f) CurrentCooldown -= DeltaTime;
	if (CurrentCooldown < 0.f)
	{
		CurrentCooldown = 0.f;
		UpdateHaveResourceForPush();
	};
	
}

void UPushActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
	Super::MyServer_ExecuteAction(Payload, Actors);

	if(!CanExecuteAction()) return;
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, MyPrisoner);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);

	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(
	RV_Hit, //result
	CameraComponent->GetComponentLocation(), //start
	CameraComponent->GetForwardVector() * PushData->RangeToPush + CameraComponent->GetComponentLocation(),
	//end
	ECC_Camera, //collision channel
	RV_TraceParams);

	if (RV_Hit.bBlockingHit)
	{
		bool CanInteract = (Cast<IPushable>(RV_Hit.GetActor()) != nullptr);

		if (!CanInteract) return;
		if (RV_Hit.GetActor()->Implements<UPushable>())
		{
			if (IPushable::Execute_CanBePushed(RV_Hit.GetActor(), MyPrisoner))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Try to Push Actor on PushActionComponent"));
				IPushable::Execute_TryPush(RV_Hit.GetActor(), PushData->PushDuration, CameraComponent->GetForwardVector(), PushData->PushForce);
				Super::MyServer_ExecuteAction(Payload, Actors);
				Multicast_ExecuteAction(Payload, Actors);
				MyPrisoner->OnPushThing();
				CurrentCooldown = PushData->PushCooldown;
				if(StaminaSystem->GetCurrentStamina() >= PushData->PushStaminaCost){
					StaminaSystem->Server_ConsumeStamina(PushData->PushStaminaCost);
				}
				else
				{
					StaminaSystem->Server_ConsumeStamina(StaminaSystem->GetCurrentStamina());
					CanConsumeMinimumStaminaCost = false;
				}
				UpdateHaveResourceForPush();
			}
		}
	}
}

void UPushActionComponent::InitServerAction(APrisoner* InPrisoner)
{
	Super::InitServerAction(InPrisoner);
	CameraComponent = InPrisoner->GetCameraComponent();
	StaminaSystem = MyPrisoner->Stamina;
	StaminaSystem->OnUpdateStamina.AddDynamic(this, &UPushActionComponent::OnUpdatedStamina);
}

bool UPushActionComponent::CanExecuteAction()
{
	return Super::CanExecuteAction() && CurrentCooldown <= 0.f && (StaminaSystem->GetCurrentStamina() >= PushData->PushStaminaCost || CanConsumeMinimumStaminaCost);
}

void UPushActionComponent::OnUpdatedStamina()
{
	if(!CanConsumeMinimumStaminaCost && StaminaSystem->GetCurrentStamina() >= PushData->PushStaminaCost)
	{
		CanConsumeMinimumStaminaCost = true;
		
	}
	UpdateHaveResourceForPush();
}

void UPushActionComponent::OnRep_HaveResourceForPush()
{
	OnHaveResourceForPushChanged.Broadcast();
}

bool UPushActionComponent::GetHaveResourceForPush()
{
	return HaveResourceForPush;
}

void UPushActionComponent::UpdateHaveResourceForPush()
{
	if(CurrentCooldown <= 0.f && (StaminaSystem->GetCurrentStamina() >= PushData->PushStaminaCost || CanConsumeMinimumStaminaCost)){
		if(!HaveResourceForPush)
		{
			HaveResourceForPush = true;
			OnRep_HaveResourceForPush();
		}
	}
	else
	{
		if(HaveResourceForPush)
		{
			HaveResourceForPush = false;
			OnRep_HaveResourceForPush();
		}
	}
}
void UPushActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPushActionComponent, HaveResourceForPush);

}