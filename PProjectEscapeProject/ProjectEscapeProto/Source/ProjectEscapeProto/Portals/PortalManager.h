// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalManager.generated.h"
class APrisoner;
class APrisonerController;
class APortal;

UCLASS

()
class PROJECTESCAPEPROTO_API APortalManager : public AActor
{
	GENERATED_BODY()	

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static APortalManager* GetPortalManager( AActor* Context );
	
	APortalManager();
	APortalManager(const FObjectInitializer& ObjectInitializer);

	// Called by a Portal actor when wanting to teleport something
	UFUNCTION(BlueprintCallable, Category="Portal")
	void RequestTeleportByPortal( APortal* Portal, AActor* TargetToTeleport );

	// Save a reference to the PlayerControler
	void SetControllerOwner( APrisonerController* NewOwner );
	

	// Manual Tick
	void Update( float DeltaTime );

	

	// Update SceneCapture
	void UpdateCapture( APortal* Portal ) const;




private:
	//Function to create the Portal render target
	void GeneratePortalTexture(APortal* Portal);


	int TextureIndex= 0;
	UTextureRenderTarget2D* TempPortalTexture;

	UPROPERTY()
	APrisonerController* ControllerOwner;

	int32 PreviousScreenSizeX;
	int32 PreviousScreenSizeY;

	float UpdateDelay;

	TWeakObjectPtr<APrisoner> Character;
};
