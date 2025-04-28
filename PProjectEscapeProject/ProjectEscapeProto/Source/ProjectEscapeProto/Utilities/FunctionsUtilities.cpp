// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionsUtilities.h"

#include <regex>
#include <string>

#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "ProjectEscapeProto/OnlineGameMode.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/IA/AICharacter.h"
#include "ProjectEscapeProto/IA/HearingNeighbour_Entity/HearingNeighbourController.h"
#include "Windows/WindowsPlatformApplicationMisc.h"


FString UFunctionsUtilities::GetRandomKey(int length)
{
	FString AllKeys = "AZERTYUIOPQSDFGHJKLMWXCVBN1234567890";
	FString NewCode = "";

	for (int i = 0; i < length; i++)
	{
		NewCode.InsertAt(i, AllKeys[FMath::RandRange(0, AllKeys.Len() - 1)]);
	}
	return NewCode;
}

FString UFunctionsUtilities::CopyValueToClip(FString quote)
{
	FPlatformApplicationMisc::ClipboardCopy(*quote);
	return quote;
}

bool UFunctionsUtilities::IsPointInsideBox(FVector Point, UBoxComponent* Box)
{
	if (Box != nullptr)
	{
		// From :
		// https://stackoverflow.com/questions/52673935/check-if-3d-point-inside-a-box/52674010

		FVector Center = Box->GetComponentLocation();
		FVector Half = Box->GetScaledBoxExtent();
		FVector DirectionX = Box->GetForwardVector();
		FVector DirectionY = Box->GetRightVector();
		FVector DirectionZ = Box->GetUpVector();

		FVector Direction = Point - Center;

		bool IsInside = FMath::Abs(FVector::DotProduct(Direction, DirectionX)) <= Half.X &&
			FMath::Abs(FVector::DotProduct(Direction, DirectionY)) <= Half.Y &&
			FMath::Abs(FVector::DotProduct(Direction, DirectionZ)) <= Half.Z;

		return IsInside;
	}
	else
	{
		return false;
	}
}

bool UFunctionsUtilities::BinaryMask_HaveValue(int32 BinaryMask, int32 Value)
{
	return BinaryMask & (1 << StaticCast<int32>(Value));
}

bool UFunctionsUtilities::BinaryMask_EqualValue(int32 FirstBinaryMask, int32 SecondBinaryMask, int32 Value)
{
	if (FirstBinaryMask != SecondBinaryMask)
	{
		return BinaryMask_HaveValue(FirstBinaryMask, Value) == BinaryMask_HaveValue(SecondBinaryMask, Value);
	}
	return true;
}

void UFunctionsUtilities::BinaryMask_SetValue(int32& BinaryMask, int32 ValueToSet, bool Value)
{
	if (Value)
	{
		BinaryMask |= 1 << ValueToSet;
	}
	else
	{
		BinaryMask &= ~(1 << ValueToSet);
	}
}

void UFunctionsUtilities::ForceRebuildNavigation(UWorld* World)
{
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null!"));
		return;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (NavSys)
	{
		// Request a rebuild
		NavSys->Build();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get Navigation System."));
	}
}

FString UFunctionsUtilities::TransliterateToLatin(FString InputString)
{
	FString Result;
    
	for (const TCHAR& Char : InputString)
	{
		if (Char == TEXT('α')) Result.Append(TEXT("a"));
		else if (Char == TEXT('β')) Result.Append(TEXT("b"));
		else if (Char == TEXT('γ')) Result.Append(TEXT("g"));
		else if (Char == TEXT('д')) Result.Append(TEXT("d"));
		else if (Char == TEXT('ж')) Result.Append(TEXT("zh"));
		else if (Char == TEXT('и')) Result.Append(TEXT("i"));
		else if (Char == TEXT('й')) Result.Append(TEXT("y"));
		else if (Char == TEXT('к')) Result.Append(TEXT("k"));
		else if (Char == TEXT('л')) Result.Append(TEXT("l"));
		else if (Char == TEXT('м')) Result.Append(TEXT("m"));
		else if (Char == TEXT('н')) Result.Append(TEXT("n"));
		else if (Char == TEXT('о')) Result.Append(TEXT("o"));
		else if (Char == TEXT('п')) Result.Append(TEXT("p"));
		else if (Char == TEXT('р')) Result.Append(TEXT("r"));
		else if (Char == TEXT('с')) Result.Append(TEXT("s"));
		else if (Char == TEXT('т')) Result.Append(TEXT("t"));
		else if (Char == TEXT('у')) Result.Append(TEXT("u"));
		else if (Char == TEXT('ф')) Result.Append(TEXT("f"));
		else if (Char == TEXT('х')) Result.Append(TEXT("kh"));
		else if (Char == TEXT('ц')) Result.Append(TEXT("ts"));
		else if (Char == TEXT('ч')) Result.Append(TEXT("ch"));
		else if (Char == TEXT('ш')) Result.Append(TEXT("sh"));
		else if (Char == TEXT('щ')) Result.Append(TEXT("shch"));
		else if (Char == TEXT('ы')) Result.Append(TEXT("y"));
		else if (Char == TEXT('э')) Result.Append(TEXT("e"));
		else if (Char == TEXT('ю')) Result.Append(TEXT("yu"));
		else if (Char == TEXT('я')) Result.Append(TEXT("ya"));
		else if (Char == TEXT('ё')) Result.Append(TEXT("yo"));
		else if (Char == TEXT('А')) Result.Append(TEXT("A"));
		else if (Char == TEXT('Б')) Result.Append(TEXT("B"));
		else if (Char == TEXT('В')) Result.Append(TEXT("V"));
		else if (Char == TEXT('Г')) Result.Append(TEXT("G"));
		else if (Char == TEXT('Д')) Result.Append(TEXT("D"));
		else if (Char == TEXT('Е')) Result.Append(TEXT("E"));
		else if (Char == TEXT('Ё')) Result.Append(TEXT("Yo"));
		else if (Char == TEXT('Ж')) Result.Append(TEXT("Zh"));
		else if (Char == TEXT('З')) Result.Append(TEXT("Z"));
		else if (Char == TEXT('И')) Result.Append(TEXT("I"));
		else if (Char == TEXT('Й')) Result.Append(TEXT("Y"));
		else if (Char == TEXT('К')) Result.Append(TEXT("K"));
		else if (Char == TEXT('Л')) Result.Append(TEXT("L"));
		else if (Char == TEXT('М')) Result.Append(TEXT("M"));
		else if (Char == TEXT('Н')) Result.Append(TEXT("N"));
		else if (Char == TEXT('О')) Result.Append(TEXT("O"));
		else if (Char == TEXT('П')) Result.Append(TEXT("P"));
		else if (Char == TEXT('Р')) Result.Append(TEXT("R"));
		else if (Char == TEXT('С')) Result.Append(TEXT("S"));
		else if (Char == TEXT('Т')) Result.Append(TEXT("T"));
		else if (Char == TEXT('У')) Result.Append(TEXT("U"));
		else if (Char == TEXT('Ф')) Result.Append(TEXT("F"));
		else if (Char == TEXT('Х')) Result.Append(TEXT("Kh"));
		else if (Char == TEXT('Ц')) Result.Append(TEXT("Ts"));
		else if (Char == TEXT('Ч')) Result.Append(TEXT("Ch"));
		else if (Char == TEXT('Ш')) Result.Append(TEXT("Sh"));
		else if (Char == TEXT('Щ')) Result.Append(TEXT("Shch"));
		else if (Char == TEXT('Ы')) Result.Append(TEXT("Y"));
		else if (Char == TEXT('Э')) Result.Append(TEXT("E"));
		else if (Char == TEXT('Ю')) Result.Append(TEXT("Yu"));
		else if (Char == TEXT('Я')) Result.Append(TEXT("Ya"));
		else if (Char == TEXT('0')) Result.Append(TEXT("0"));
		else if (Char == TEXT('1')) Result.Append(TEXT("1"));
		else if (Char == TEXT('2')) Result.Append(TEXT("2"));
		else if (Char == TEXT('3')) Result.Append(TEXT("3"));
		else if (Char == TEXT('4')) Result.Append(TEXT("4"));
		else if (Char == TEXT('5')) Result.Append(TEXT("5"));
		else if (Char == TEXT('6')) Result.Append(TEXT("6"));
		else if (Char == TEXT('7')) Result.Append(TEXT("7"));
		else if (Char == TEXT('8')) Result.Append(TEXT("8"));
		else if (Char == TEXT('9')) Result.Append(TEXT("9"));
		else if (FChar::IsDigit(Char) || FChar::IsAlpha(Char) || FChar::IsAlnum(Char)) Result.AppendChar(Char);
	}
    
	return Result;
}

FString UFunctionsUtilities::NormalizeString(FString InputString)
{
	FString Intermediate = TransliterateToLatin(InputString);
	
	std::string IntermediateStd = TCHAR_TO_UTF8(*Intermediate);
	std::regex AlnumRegex("[^a-zA-Z0-9]");
	std::string Cleaned = std::regex_replace(IntermediateStd, AlnumRegex, "");
	
	return FString(UTF8_TO_TCHAR(Cleaned.c_str()));
}

char UFunctionsUtilities::GetPointOnFloor(APrisoner* Caster, float MaxDistance, FVector& NewLocation)
{	
	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	AActor* ACaster = Cast<AActor>(Caster);
	CollisionParams.AddIgnoredActor(ACaster);
	FVector StartPos = Caster->GetCameraPosition();
	FVector EndLocation = StartPos + Caster->GetCameraForwardVector() * 1000;
	NewLocation = StartPos + Caster->GetCameraForwardVector() * 200;

	if (Caster->GetWorld()->LineTraceSingleByChannel(HitResult, StartPos, EndLocation, ECC_Visibility, CollisionParams))
	{
		NewLocation = HitResult.Location;
		if (FVector::Dist(HitResult.Location, StartPos) > MaxDistance) return 1;
	}
	else
		return 2;
	if (FVector::DotProduct(HitResult.ImpactNormal, FVector::UpVector) < 0.9f) // Cos(25)
		return 2;

	return 0;
}

TArray<ERoomType> UFunctionsUtilities::GetAllRoomTypes()
{
	return { ERoomType::RSM, ERoomType::Suburbs, ERoomType::Mall, ERoomType::GasStation, ERoomType::Office };
}


void UFunctionsUtilities::Server_GivePointToPlayer(AActor* WorldContext, int PlayerId, int Point)
{
	if (!WorldContext->HasAuthority()) return;
	AOnlineGameMode* GM = Cast<AOnlineGameMode>(WorldContext->GetWorld()->GetAuthGameMode());
	if (!GM) return;

	GM->AskUpdateScore(Point, PlayerId);
}

APrisoner* UFunctionsUtilities::Server_GetPlayerWithId(AActor* WorldContext, int PlayerId)
{
	AGameStateBase* GameState = WorldContext->GetWorld()->GetGameState();
	if (!GameState) return nullptr;

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (PlayerState->GetPlayerId() == PlayerId)
		{
			return Cast<APrisoner>(PlayerState->GetPawn());
		}
	}
	return nullptr;
}

void UFunctionsUtilities::Server_MakeNoise(AActor* WorldContext, AActor* Instigator, FVector Location, float Loudness,
	int32 MaxRange)
{
	TArray<AActor*> AllAI;
	UGameplayStatics::GetAllActorsOfClass(WorldContext->GetWorld(), AAICharacter::StaticClass(), AllAI);
	for (AActor* AI : AllAI)
	{
		AAICharacter* AICharacter = Cast<AAICharacter>(AI);
		if (!AICharacter) continue;
		
		AHearingNeighbourController *AIController = Cast<AHearingNeighbourController>(AICharacter->GetController());
		if (!AIController) continue;
		
		AIController->LastHearingSoundStrength = Loudness;
		AIController->LastHearingSoundActor = Instigator;
	}
}