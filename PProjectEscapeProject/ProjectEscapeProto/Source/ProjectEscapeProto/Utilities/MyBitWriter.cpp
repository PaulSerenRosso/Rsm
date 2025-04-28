// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBitWriter.h"

#include "Engine/PackageMapClient.h"




void UMyBitWriter::Init()
{
	Data.Empty();
}

TArray<uint8> UMyBitWriter::GetData()
{
	return Data;
}

void UMyBitWriter::SetString(FString src)
{
	Data.Append((uint8*)TCHAR_TO_UTF8(*src), src.Len());
}

void UMyBitWriter::SetBool(bool  src)
{
	Data.Add(src == true ? 1 : 0);
}

void UMyBitWriter::SetFloat(float src)
{
	Data.Append(reinterpret_cast<uint8*>(&src), sizeof(src));
}

void UMyBitWriter::SetInt(int src)
{
	Data.Append(reinterpret_cast<uint8*>(&src), sizeof(src));
}

void UMyBitWriter::SetUInt64(uint64 src)
{
	Data.Append(reinterpret_cast<uint8*>(&src), sizeof(src));
}

void UMyBitWriter::SetVector2D(FVector2D src)
{
	SetFloat(src.X);
	SetFloat(src.Y);
}






