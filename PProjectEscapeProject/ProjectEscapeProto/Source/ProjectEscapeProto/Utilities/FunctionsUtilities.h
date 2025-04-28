// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumsUtilities.h"
#include "UObject/NoExportTypes.h"
#include "FunctionsUtilities.generated.h"


class APrisoner;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API UFunctionsUtilities : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetRandomKey(int length);
	UFUNCTION(BlueprintCallable)
	static FString CopyValueToClip(FString quote);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsPointInsideBox(FVector Point, UBoxComponent* Box);

	template <typename To, typename From>
	static TArray<To*> ShuffleArray(TArray<From*> Array);
	template <typename To, typename From>
	static TArray<To> ShuffleArray(TArray<From> Array);
	UFUNCTION(BlueprintPure)
	static bool BinaryMask_HaveValue(int32 BinaryMask, int32 Value);
	UFUNCTION(BlueprintPure)
	static bool BinaryMask_EqualValue(int32 FirstBinaryMask, int32 SecondBinaryMask, int32 Value);
	UFUNCTION(BlueprintCallable)
	static void BinaryMask_SetValue(int32& BinaryMask, int32 ValueToSet, bool Value);
	UFUNCTION(BlueprintCallable)
	static void ForceRebuildNavigation(UWorld* World);
	
	static FString TransliterateToLatin(FString InputString);
	static FString NormalizeString(FString InputString);
	static char GetPointOnFloor(APrisoner* Caster, float MaxDistance, FVector& NewLocation); // 0 = Success, 1 = Fail, 2 = No Floor
	static TArray<ERoomType> GetAllRoomTypes();
	UFUNCTION(BlueprintCallable)
	// static void GivePointToPlayer(APrisoner *Player, int Point);
	// UFUNCTION(Server, Reliable)
	static void Server_GivePointToPlayer(AActor* WorldContext, int PlayerId, int Point);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static APrisoner* Server_GetPlayerWithId(AActor* WorldContext, int PlayerId);
	UFUNCTION(BlueprintCallable)
	static void Server_MakeNoise(AActor* WorldContext, AActor* Instigator, FVector Location, float Loudness, int32 MaxRange = 0);

	template <typename T>
	static TArray<T> GetArraySlice(const TArray<T>& SourceArray, int32 StartIndex, int32 EndIndex);
};


template <typename To, typename From>
TArray<To*> UFunctionsUtilities::ShuffleArray(TArray<From*> Array)
{
	if (Array.Num() > 0)
	{
		int32 LastIndex = Array.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Array.Swap(i, Index);
			}
		}
	}
	return Array;
}

template <typename To, typename From>
TArray<To> UFunctionsUtilities::ShuffleArray(TArray<From> Array)
{
	if (Array.Num() > 0)
	{
		int32 LastIndex = Array.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Array.Swap(i, Index);
			}
		}
	}
	return Array;
}

template <typename T>
TArray<T> UFunctionsUtilities::GetArraySlice(const TArray<T>& SourceArray, int32 StartIndex, int32 EndIndex)
{
	TArray<T> SubArray;

	// Ensure the start and end indices are valid
	if (StartIndex >= 0 && EndIndex < SourceArray.Num() && StartIndex <= EndIndex)
	{
		// Copy elements from the original array to the subarray
		for (int32 i = StartIndex; i <= EndIndex; ++i)
		{
			SubArray.Add(SourceArray[i]);
		}
	}

	return SubArray;
}
