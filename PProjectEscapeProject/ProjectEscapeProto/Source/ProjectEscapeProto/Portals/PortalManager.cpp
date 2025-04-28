// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"
#include "EngineUtils.h"
#include "Portal.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/Characters/PrisonerController.h"




APortalManager* APortalManager::GetPortalManager(AActor* Context)
{
	APortalManager* Manager = nullptr;

	//Retrieve the World from the Context actor
	if( Context != nullptr && Context->GetWorld() != nullptr )
	{
		//Find PlayerController
		APrisonerController* EPC = Cast<APrisonerController>( Context->GetWorld()->GetFirstPlayerController() );

		//Retrieve the Portal Manager
		// if( EPC != nullptr && EPC->PortalManager != nullptr )
		// {
		// 	Manager = EPC->PortalManager;
		// }
	}

	return Manager;
}

// Sets default values
APortalManager::APortalManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


APortalManager::APortalManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	TempPortalTexture = nullptr;
	UpdateDelay = 1.1f;

	PreviousScreenSizeX = 0;
	PreviousScreenSizeY = 0;
}

void APortalManager::SetControllerOwner(APrisonerController* NewOwner)
{
	ControllerOwner = NewOwner;

	// My PlayerController class has a function to retrieve the associated 
	// Character class for convenience. You will need to adapt this code
	// to get your own. You can use UGameplayStatics::GetPlayerCharacter()
	// for example, by casting your class on top of course.
	Character = Cast<APrisoner>(NewOwner->GetCharacter());
}



void APortalManager::GeneratePortalTexture(APortal* Portal)
{
	TempPortalTexture = Portal->PortalTexture;
	int32 CurrentSizeX = 1920;
	int32 CurrentSizeY = 1080;

	if (ControllerOwner != nullptr)
	{
		ControllerOwner->GetViewportSize(CurrentSizeX, CurrentSizeY);
	}

	// Create the RenderTarget if it does not exist
	if (TempPortalTexture == nullptr)
	{
		// Create new RTT
		TempPortalTexture = NewObject<UTextureRenderTarget2D>(
			this,
			UTextureRenderTarget2D::StaticClass(),
			*FString("PortalRenderTarget"+TextureIndex)
		);
		TextureIndex++;
		check(TempPortalTexture);
		
		 
		 TempPortalTexture->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
		 TempPortalTexture->Filter = TextureFilter::TF_MAX;
		 TempPortalTexture->SizeX = CurrentSizeX;
		 TempPortalTexture->SizeY = CurrentSizeY;
		 TempPortalTexture->ClearColor = FLinearColor::Black;
		 TempPortalTexture->TargetGamma = 2.2f;
		 TempPortalTexture->bNeedsTwoCopies = false;
		 TempPortalTexture->AddressX = TextureAddress::TA_Clamp;
		TempPortalTexture->AddressY = TextureAddress::TA_Clamp;

		// Not needed since the texture is displayed on screen directly
		// in some engine versions this can even lead to crashes (notably 4.24/4.25)
		TempPortalTexture->bAutoGenerateMips = false;

		// This force the engine to create the render target 
		// with the parameters we defined just above
		TempPortalTexture->UpdateResource();
		Portal->PortalTexture = TempPortalTexture;
		Portal->SetRTT(	Portal->PortalTexture);
		Portal->SceneCapture->TextureTarget = Portal->PortalTexture;
	}
	// Resize the RenderTarget if it already exists
	else
	{
		TempPortalTexture->ResizeTarget(CurrentSizeX,CurrentSizeY);
	}
}

void APortalManager::Update(float DeltaTime)
{
	// for (TActorIterator<APortal> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	// {
	// 	APortal* Portal = *ActorItr;
	// 	GeneratePortalTexture(Portal);
	// 	UpdateCapture(Portal);
	// }
}



void APortalManager::UpdateCapture(APortal* Portal) const
{
	if (ControllerOwner == nullptr)
	{
		return;
	}

	//-----------------------------------
	// Update SceneCapture (discard if there is no active portal)
	//-----------------------------------
	if (Portal->SceneCapture != nullptr
		&& Portal != nullptr
		&& Character != nullptr)
	{
		UCameraComponent* PlayerCamera = Character->GetCameraComponent();
		
		AActor* Target = Portal->GetTarget();

		//Place the SceneCapture to the Target
		if (Target != nullptr)
		{
			//-------------------------------
			// Compute new location in the space of the target actor
			// (which may not be aligned to world)
			//-------------------------------
			FVector NewLocation = Target->GetTransform().TransformPosition(
				Portal->GetTransform().InverseTransformPosition(PlayerCamera->GetComponentLocation()));

		

			//-------------------------------
			//Compute new Rotation in the space of the
			//Target location
			//-------------------------------
			FTransform CameraTransform = PlayerCamera->GetComponentTransform();
			FTransform SourceTransform = Portal->GetActorTransform();
			FTransform TargetTransform = Target->GetActorTransform();
			

			//-------------------------------
			//Clip Plane : to ignore objects between the
			//SceneCapture and the Target of the portal
			//-------------------------------
			Portal->SceneCapture->ClipPlaneNormal = Target->GetActorForwardVector()* -1.5f;
			Portal->SceneCapture->ClipPlaneBase = Target->GetActorLocation()
				+ (Portal->SceneCapture->ClipPlaneNormal); //Offset to avoid visible pixel border

		// Get the Projection Matrix
		Portal->SceneCapture->CustomProjectionMatrix = ControllerOwner->GetCameraProjectionMatrix();
			Portal->SceneCapture->FOVAngle =Character->GetCameraComponent()->FieldOfView;
			Portal->SceneCapture->SetWorldLocation(NewLocation);
			//Update SceneCapture rotation
			Portal->SceneCapture->SetWorldRotation(TargetTransform.TransformRotation(SourceTransform.InverseTransformRotation(CameraTransform.GetRotation())));
		}
		
		

	
	}
}

void APortalManager::RequestTeleportByPortal(APortal* Portal, AActor* TargetToTeleport)
{
	if (Portal != nullptr && TargetToTeleport != nullptr)
	{
		if(ControllerOwner != nullptr)
		{
			ControllerOwner->PerformCameraCut();
		}
		Portal->TeleportActor(TargetToTeleport);

		//-----------------------------------
		//Force update
		//-----------------------------------
		
	}
}
