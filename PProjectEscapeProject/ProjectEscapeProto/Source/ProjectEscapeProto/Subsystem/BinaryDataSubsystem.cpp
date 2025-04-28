// Fill out your copyright notice in the Description page of Project Settings.


#include "BinaryDataSubsystem.h"

#include "ProjectEscapeProto/Utilities/MyBitReader.h"
#include "ProjectEscapeProto/Utilities/MyBitWriter.h"

void UBinaryDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	BitWriter = NewObject<UMyBitWriter>();

	BitReader = NewObject<UMyBitReader>();

	
	
}



