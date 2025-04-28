// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Channel3DProperties.h"
#include "ChannelId.h"
#include "StructUtilities.generated.h"

class AItem;
class ASpawnActor;

#define MAX_NOISE 5000.0f

USTRUCT(BlueprintType)
struct FPlayerScore
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, DisplayName = "PlayerId")
	int PlayerId;
	UPROPERTY(BlueprintReadWrite, DisplayName = "PlayerName")
	FString PlayerName;
	UPROPERTY(BlueprintReadWrite, DisplayName = "Score")
	int Score;

	FPlayerScore(): PlayerId(0), Score(0)
	{
	}

	FPlayerScore(int playerId, int score): PlayerId(playerId), Score(score)
	{
	}
};

USTRUCT(BlueprintType)
struct FMyChannelProperties
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, DisplayName = "Audible Distance", Category = "3D Properties")
	int32 audibleDistance = 2700;

	UPROPERTY(BlueprintReadWrite, DisplayName = "Conversational Distance", Category = "3D Properties")
	int32 conversationalDistance = 90;

	UPROPERTY(BlueprintReadWrite, DisplayName = "Audio Fade Intensity By Distance", Category = "3D Properties")
	double audioFadeIntensityByDistance = 1.0;
	UPROPERTY(BlueprintReadWrite, DisplayName = "Audio Fade Mode", Category = "3D Properties")
	EAudioFadeModel audioFadeMode = EAudioFadeModel::InverseByDistance;

	UPROPERTY(BlueprintReadWrite, DisplayName = "Audio Fade Mode", Category = "Channel Properties")
	ChannelType InputChannelType = ChannelType::Positional;
	UPROPERTY(BlueprintReadWrite, DisplayName = "Audio Fade Mode", Category = "Channel Properties")
	bool ConnectAudio = true;
	UPROPERTY(BlueprintReadWrite, DisplayName = "Audio Fade Mode", Category = "Channel Properties")
	bool ConnectText = false;
	UPROPERTY(BlueprintReadWrite, DisplayName = "Audio Fade Mode", Category = "Channel Properties")
	bool SwitchTransmition;


	FMyChannelProperties()

	{
	}
};

USTRUCT(BlueprintType)
struct FActorSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TSubclassOf<ASpawnActor> ActorType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int NbToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FString TagOfSpawnedActors;
};

USTRUCT(BlueprintType)
struct FSavePrisonerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FString FirstItem = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FString SecondItem = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	int Score = 0;
};

USTRUCT(BlueprintType)
struct FGenericArrayVector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	TArray<FVector> ArrayVector;
};

USTRUCT(BlueprintType)
struct FGenericFloatGoal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float Goal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float Current;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float StepTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	float Duration;
};

USTRUCT(BlueprintType)
struct FColorPlayer
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	int PlayerId = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FLinearColor Color = FLinearColor::White;
};

UCLASS()
class PROJECTESCAPEPROTO_API UStructUtilities : public UObject
{
	GENERATED_BODY()
};
