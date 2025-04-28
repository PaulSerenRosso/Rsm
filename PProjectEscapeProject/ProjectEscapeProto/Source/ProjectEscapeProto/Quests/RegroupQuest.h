#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "RegroupQuest.generated.h"


class APlayerGroupPoint;
class AOnlineGameMode;
class URegroupQuestData;
class APrisoner;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API URegroupQuest : public UQuest
{
	GENERATED_BODY()
protected:
	virtual void StartQuest(UWorld* World) override;
	UFUNCTION()
	void OnPlayerLeaveGroup(APrisoner* Prisoner);
	UFUNCTION()
	void OnPlayerJoinGroup(APrisoner* Prisoner);
	
protected:
	
	APlayerGroupPoint* PlayerGroupPoint;
	URegroupQuestData* RegroupQuestData;
	AOnlineGameMode* GameMode;
	
};
