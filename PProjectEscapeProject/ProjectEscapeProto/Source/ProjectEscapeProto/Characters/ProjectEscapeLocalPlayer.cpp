// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectEscapeLocalPlayer.h"

UProjectEscapeLocalPlayer::UProjectEscapeLocalPlayer()
{
	bCameraCut = false;
}

FSceneView* UProjectEscapeLocalPlayer::CalcSceneView(class FSceneViewFamily* ViewFamily, FVector& OutViewLocation,
	FRotator& OutViewRotation, FViewport* Viewport, class FViewElementDrawer* ViewDrawer, int32 StereoViewIndex)
{
	FSceneView* View = Super::CalcSceneView(ViewFamily, OutViewLocation, OutViewRotation, Viewport, ViewDrawer, StereoViewIndex);

	if( bCameraCut )
	{
		View->bCameraCut = true;
		bCameraCut = false;
	}
	return View;
}


void UProjectEscapeLocalPlayer::PerformCameraCut()
{
	bCameraCut = true;
}