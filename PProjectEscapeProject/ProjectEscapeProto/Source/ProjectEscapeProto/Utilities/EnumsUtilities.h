// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EnumsUtilities.generated.h"

/**
 * 
 */


UENUM(BlueprintType, Meta = (Bitflags))
enum class EPlayerStates: uint8
{
	Dead, Spectator,
};

UENUM(BlueprintType, Meta = (Bitflags))
enum class EPlayerActionState: uint8
{
	None,
	IsWalkingQuiet,
	IsSprinting,
	IsCrouching,
	IsGrabbingItem,
};

UENUM(BlueprintType, Meta = (Bitflags))
enum EMascotKnowledgeType : uint8
{
	Trap,
	MainQuestStart,
	AllQuestsEnded,
	TypeOfQuest,
	PrisonerEnteredInElevator,
	NbPrisonerEnteredInElevator,
	ElevatorClosed
};

UENUM(BlueprintType, Meta = (Bitflags))
enum EMascotTag : uint8
{
	None,
	ArmTrapTwice,
	AsMainQuestStart,
	IsGeneratorQuest,
	MainQuestSuccess,
	MainQuestFail,
	IsPrisonerEnteredInElevator,
	IsPrisonerFailToEnterOnElevator
};

UENUM(BlueprintType, Meta = (Bitflags))
enum class ERoomType : uint8
{
	RSM,
	Suburbs,
	Mall,
	GasStation,
	Office,
	ERoomType_NONE
};

UENUM(BlueprintType, Meta = (Bitflags))
enum class EEntityState : uint8
{
	Patrol,
	Chase,
	Hunt,
};

UCLASS()
class PROJECTESCAPEPROTO_API UEnumsUtilities : public UObject
{
	GENERATED_BODY()
};
