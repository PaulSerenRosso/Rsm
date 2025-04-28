// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VivoxCore.h"
#include "Components/ActorComponent.h"
#include "ProximityChat.generated.h"


class UOnlineGameInstance;
class UOnlineGameVivoxSubsystem;
class UProximityChatData;
struct FMyChannelProperties;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnParticipantVolumeUpdated, const TArray<FString>&, ParticipantsName,
                                             const TArray<float>&, ParticipantsVolume);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class PROJECTESCAPEPROTO_API UProximityChat : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnParticipantVolumeUpdated OnParticipantVolumeUpdated;
	UFUNCTION
	(Client, Reliable)
	void JoinProximityChannel(const FString& KeyCode);
	void Init(bool IsLocal);

	

private:
	UProximityChat();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void TickPosition(APawn* PlayerPawn);
	void OnParticipantAdded(const IParticipant& NewParticipant);
	void OnParticipantUpdate(const IParticipant& NewParticipant);
	void OnParticipantRemoved(const IParticipant& RemovedParticipant);
	void RefreshParticipantsVolumePrint(FString ParticipantNameToAvoid = "");
	UFUNCTION(BlueprintCallable)
	void ModifyParticipantsVolume(FString ParticipantName, float Volume);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void OnBeginConnectedChannel(FString ChannelName);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	bool IsConnectedToProximityChat();

public:
	bool IsLocalProximityChat = false;

protected:
	TWeakObjectPtr<UOnlineGameVivoxSubsystem> OnlineGameVivoxSubsystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProximityChat")
	UProximityChatData* ProximityChatData;
	IChannelSession* CurrentSession;
	FString ProximityChannelName;
	TWeakObjectPtr<UOnlineGameInstance> OnlineGameInstance;
	IParticipant *Mine = nullptr;


private:
	TWeakObjectPtr<UPawnNoiseEmitterComponent> SoundEmitter;
};
