// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstLoadingManager.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectEscapeProto/OnlineGameInstance.h"


// Sets default values
AFirstLoadingManager::AFirstLoadingManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFirstLoadingManager::BeginInitCPP()
{
	BP_BeginInit();
	UOnlineGameInstance* GI = Cast<UOnlineGameInstance>(GetGameInstance());
	if(NeedToInitVivox && GI && GI->NeedToInitVivox)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(TEXT("r.VSync = 1"));
		OnlineGameVivoxSubsystem = GetGameInstance()->GetSubsystem<UOnlineGameVivoxSubsystem>();
		OnlineGameVivoxSubsystem->OnVivoxInitialized.AddDynamic(this, &AFirstLoadingManager::OnVivoxInitialized);
		OnlineGameVivoxSubsystem->InitVivox();
	}
	else
	{
		BP_EndInit();
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Menu"), true);
	}
}

void AFirstLoadingManager::OnVivoxInitialized()
{
	UOnlineGameInstance* GI = Cast<UOnlineGameInstance>(GetGameInstance());
	if(GI) GI->NeedToInitVivox = false;
	BP_EndInit();
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Menu"), true);
}

// Called every frame
void AFirstLoadingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

