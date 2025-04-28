// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class PROJECTESCAPEPROTO_API APortal : public AActor
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

public:

	APortal();
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintNativeEvent, Category="Portal")
	void ForceTick();

	//Target of where the portal is looking
	UFUNCTION(BlueprintPure, Category="Portal")
	AActor* GetTarget();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Portal")
	void SetRTT( UTexture* RenderTexture );
	
	UFUNCTION(BlueprintCallable, Category="Portal")
	void SetTarget( AActor* NewTarget );

	//Helpers
	UFUNCTION(BlueprintCallable, Category="Portal")
	bool IsPointInFrontOfPortal( FVector Point, FVector PortalLocation, FVector PortalNormal );
	

	UFUNCTION(BlueprintCallable, Category="Portal")
	void TeleportActor( AActor* ActorToTeleport );

public:
	
	UPROPERTY(transient)
	UTextureRenderTarget2D* PortalTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal")
	USceneCaptureComponent2D* SceneCapture;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	USceneComponent* PortalRootComponent;

private:

	AActor* Target;

	//Used for Tracking movement of a point
	FVector LastPosition;
	bool    LastInFront;
};
