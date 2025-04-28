
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BalloonData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UBalloonData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BalloonQuest")
	float MinTimeToFillingBalloon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BalloonQuest")
	float MaxTimeToFillingBalloon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BalloonQuest")
	int ScoreAmountPerBalloon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BalloonQuest")
	float BornMinAcceptedTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BalloonQuest")
	float BornMaxAcceptedTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BalloonQuest")
	int MaxNbPlayerInteracting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BalloonQuest")
	float DeflateTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BalloonQuest")
	float LoudnessNoiseEmitted;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BalloonQuest")
	float MaxRangeNoiseEmitted;
};
