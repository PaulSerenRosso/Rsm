// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"


// Sets default values
APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;
	SetTickGroup(TG_PostUpdateWork);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->Mobility = EComponentMobility::Static;

	PortalRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PortalRootComponent"));
	PortalRootComponent->SetupAttachment(GetRootComponent());
	PortalRootComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PortalRootComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	PortalRootComponent->Mobility = EComponentMobility::Movable;

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("PortalSceneCapture");
	SceneCapture->SetupAttachment(PortalRootComponent);
	 SceneCapture->bAlwaysPersistRenderingState = true;
	 SceneCapture->bCaptureEveryFrame = true;
	 SceneCapture->bCaptureOnMovement = true; 
	 SceneCapture->TextureTarget = nullptr;
	 SceneCapture->bEnableClipPlane = true;
	 SceneCapture->bUseCustomProjectionMatrix = true;
	SceneCapture->bUseRayTracingIfEnabled = true;
	 SceneCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	 FPostProcessSettings PostProcessSettings;
	 PostProcessSettings.bOverride_DynamicGlobalIlluminationMethod = true;
	PostProcessSettings.bOverride_ReflectionMethod = true;
	 PostProcessSettings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Type::None;
	 PostProcessSettings.ReflectionMethod = EReflectionMethod::Type::None;
	SceneCapture->PostProcessSettings = PostProcessSettings;
}




void APortal::ForceTick_Implementation()
{
}



// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
}

AActor* APortal::GetTarget()
{
	return Target;
}

void APortal::SetRTT_Implementation(UTexture* RenderTexture)
{
}


void APortal::SetTarget(AActor* NewTarget)
{
	Target = NewTarget;
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APortal::IsPointInFrontOfPortal(FVector Point, FVector PortalLocation, FVector PortalNormal)
{
	FPlane PortalPlane = FPlane(PortalLocation, PortalNormal);
	float PortalDot = PortalPlane.PlaneDot(Point);

	//If < 0 means we are behind the Plane
	//See : http://api.unrealengine.com/INT/API/Runtime/Core/Math/FPlane/PlaneDot/index.html
	return (PortalDot >= 0);
}




void APortal::TeleportActor(AActor* ActorToTeleport)
{
	if (ActorToTeleport == nullptr || Target == nullptr)
	{
		return;
	}

	//-------------------------------
	//Retrieve and save Player Velocity
	//(from the Movement Component)
	//-------------------------------
	FVector SavedVelocity = FVector::ZeroVector;
	APrisoner* EC = nullptr;

	if (ActorToTeleport->IsA(APrisoner::StaticClass()))
	{
		EC = Cast<APrisoner>(ActorToTeleport);

		SavedVelocity = EC->GetCharacterMovement()->Velocity;
	}

	//-------------------------------
	//Compute and apply new location
	//-------------------------------
	FHitResult HitResult;
	FVector NewLocation = Target->GetTransform().TransformPosition(
		this->GetActorTransform().InverseTransformPosition(ActorToTeleport->GetActorLocation()));


	ActorToTeleport->SetActorLocation(NewLocation,
	                                  false,
	                                  &HitResult,
	                                  ETeleportType::TeleportPhysics);

	//-------------------------------
	//Compute and apply new rotation
	//-------------------------------
	FRotator NewRotation = Target->GetActorTransform().TransformRotation(
									  this->GetActorTransform().InverseTransformRotation(
										  ActorToTeleport->GetActorRotation().Quaternion())).
								  Rotator();

	//Apply new rotation
	ActorToTeleport->SetActorRotation(NewRotation);

	//-------------------------------
	//If we are teleporting a character we need to
	//update its controller as well and reapply its velocity
	//-------------------------------
	if (ActorToTeleport->IsA(APrisoner::StaticClass()))
	{
		//Update Controller
		AController* EPC = EC->GetController();

		if (EPC != nullptr)
		{
			NewRotation = Target->GetActorTransform().TransformRotation(
				                      this->GetActorTransform().InverseTransformRotation(
					                      EPC->GetControlRotation().Quaternion())).
			                      Rotator();

			EPC->SetControlRotation(NewRotation);
		}

		//Reapply Velocity (Need to reorient direction into local space of Portal)
		{
			FVector Dots;
			Dots.X = FVector::DotProduct(SavedVelocity, GetActorForwardVector());
			Dots.Y = FVector::DotProduct(SavedVelocity, GetActorRightVector());
			Dots.Z = FVector::DotProduct(SavedVelocity, GetActorUpVector());

			FVector NewVelocity = Dots.X * Target->GetActorForwardVector()
				+ Dots.Y * Target->GetActorRightVector()
				+ Dots.Z * Target->GetActorUpVector();

			EC->GetMovementComponent()->Velocity = NewVelocity;
		}
	}

	//Cleanup Teleport
	LastPosition = NewLocation;
}
