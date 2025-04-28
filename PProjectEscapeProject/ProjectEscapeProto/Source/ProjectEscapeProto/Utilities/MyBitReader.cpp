// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBitReader.h"

#include "FunctionsUtilities.h"
#include "InstanceDataSceneProxy.h"
#include "Engine/PackageMapClient.h"


void UMyBitReader::Init(const TArray<uint8>& InData)
{
	Data = TArray(InData);
	Offset = 0;
}

FString UMyBitReader::GetNextStringValue()
{
	int Length = GetNextIntValue();
	TArray<uint8> TempData = UFunctionsUtilities::GetArraySlice(Data, Offset, Offset
	                                                            + Length);
	FString StringValue = FString(UTF8_TO_TCHAR(reinterpret_cast<const char*>(&Data[Offset])));
	IncreaseOffset(Length * 8);
	return StringValue;
}

float UMyBitReader::GetNextFloatValue()
{
	float FloatValue = *reinterpret_cast<const float*>(&Data[Offset]);
	IncreaseOffset(sizeof(float));
	return FloatValue;
}

int UMyBitReader::GetNextIntValue()
{
	int IntValue = *reinterpret_cast<const int32*>(&Data[Offset]);
	IncreaseOffset(sizeof(int));
	return IntValue;
}

bool UMyBitReader::GetNextBoolValue()
{
	bool BoolValue = false;
	IncreaseOffset(1);
	return BoolValue;
}

FVector2D UMyBitReader::GetNextVector2DValue()
{
	FVector2D Vector2DValue;
	float x = GetNextFloatValue();
	float y = GetNextFloatValue();

	Vector2DValue.Set(x, y);
	return Vector2DValue;
}


uint64 UMyBitReader::GetNextUInt64Value()
{
	uint64 IntValue = *reinterpret_cast<const uint64*>(&Data[Offset]);
	IncreaseOffset(sizeof(uint64));
	return IntValue;
}

void UMyBitReader::IncreaseOffset(int Amount)
{
	Offset += Amount;
}
