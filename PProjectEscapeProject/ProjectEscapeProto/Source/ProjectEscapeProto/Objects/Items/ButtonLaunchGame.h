// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ButtonLaunchGame.generated.h"

class UBoxComponent;
class UUserWidget;
class UTextRenderComponent;
/**
 * 
 */
UCLASS()
class PROJECTESCAPEPROTO_API AButtonLaunchGame : public AItem
{
	GENERATED_BODY()

public:
	AButtonLaunchGame();
	virtual void BeginPlay() override;
	virtual bool TryInteract_Implementation(AActor* Actor) override;
	virtual bool TryStopInteract_Implementation(AActor* Actor) override;
	
	UFUNCTION(Server, Reliable)
	void Server_RefreshNbPlayerReady();

protected:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateRenderText(const FString& Text);
	
	//UFUNCTION(Client, Reliable)
	void Client_RequestRefresh();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateWidgetText(const FString& Text);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartCountdown(float _CountDownTimer);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartCountdown(float _CountDownTimer);

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ReadySystem")
	UTextRenderComponent* TextRender;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ReadySystem")
	UWidgetComponent* WidgetComponent; // Référence au Widget Component
	
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "ReadySystem")
	int32 NbPlayerReady;

	
	UPROPERTY(Replicated)
	int32 NbPlayersNeedToPlay;
	
	UPROPERTY(Replicated)
	bool IsGameReady = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReadySystem")
	float CountDownTimer = 10.0f;
};
