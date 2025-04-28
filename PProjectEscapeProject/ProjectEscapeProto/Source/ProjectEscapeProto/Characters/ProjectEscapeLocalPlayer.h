// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "ProjectEscapeLocalPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UProjectEscapeLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
	UProjectEscapeLocalPlayer();

public:
	virtual FSceneView* CalcSceneView(class FSceneViewFamily* ViewFamily, FVector& OutViewLocation, FRotator& OutViewRotation, FViewport* Viewport, class FViewElementDrawer* ViewDrawer, int32 StereoViewIndex) override;

	void PerformCameraCut();

private:
	bool bCameraCut;
};
