// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractActionComponent.h"
#include "Camera/CameraComponent.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/DataAssets/TweakeableValueCharacterData.h"
#include "ProjectEscapeProto/Interfaces/IInteractable.h"
#include "ProjectEscapeProto/Objects/InteractableObjects.h"

#define ECC_Interactable ECC_GameTraceChannel3 

// Sets default values for this component's properties
UInteractActionComponent::UInteractActionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    InteractableChannel = ECC_Interactable;
}

// Called when the game starts
void UInteractActionComponent::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("InteractableChannel initialized: %d"), (int32)InteractableChannel);
}

void UInteractActionComponent::MyServer_ExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
    if (!CanExecuteAction()) return;

    FCollisionQueryParams RV_TraceParams(FName(TEXT("RV_Trace")), true, MyPrisoner);
    RV_TraceParams.bTraceComplex = true;
    RV_TraceParams.bReturnPhysicalMaterial = false;

    FHitResult RV_Hit;
    FVector Start = CameraComponent->GetComponentLocation();
    FVector End = Start + (CameraComponent->GetForwardVector() * CharacterData->InteractRange);

    // Execute LineTrace
    bool bHit = GetWorld()->LineTraceSingleByChannel(RV_Hit, Start, End, ECC_Camera, RV_TraceParams);

    if (bHit)
    {
        if (RV_Hit.GetActor() && RV_Hit.GetActor()->Implements<UIInteractable>())
        {
            if (IIInteractable::Execute_CanInteract(RV_Hit.GetActor(), MyPrisoner))
            {
                IIInteractable::Execute_TryInteract(RV_Hit.GetActor(), MyPrisoner);
                Super::MyServer_ExecuteAction(Payload, Actors);
                Multicast_ExecuteAction(Payload, Actors);
            }
        }
    }
    else
    {
        
    }
}

void UInteractActionComponent::MyServer_StopExecuteAction(const TArray<uint8>& Payload, const TArray<AActor*>& Actors)
{
    Super::MyServer_StopExecuteAction(Payload, Actors);

    FCollisionQueryParams RV_TraceParams(FName(TEXT("RV_Trace")), true, MyPrisoner);
    RV_TraceParams.bTraceComplex = true;
    RV_TraceParams.bReturnPhysicalMaterial = false;

    FHitResult RV_Hit;
    FVector Start = CameraComponent->GetComponentLocation();
    FVector End = Start + (CameraComponent->GetForwardVector() * CharacterData->InteractRange);

    bool bHit = GetWorld()->LineTraceSingleByChannel(RV_Hit, Start, End, ECC_Camera, RV_TraceParams);

    if (bHit && RV_Hit.GetActor())
    {
        if (RV_Hit.GetActor()->Implements<UIInteractable>())
        {
            if (IIInteractable::Execute_CanInteract(RV_Hit.GetActor(), MyPrisoner))
            {
                IIInteractable::Execute_TryStopInteract(RV_Hit.GetActor(), MyPrisoner);
                Super::MyServer_StopExecuteAction(Payload, Actors);
                Multicast_StopExecuteAction(Payload, Actors);
            }
        }
    }
    else
    {
        
    }
}

void UInteractActionComponent::InitMulticastAction(APrisoner* InPrisoner)
{
    Super::InitMulticastAction(InPrisoner);
    CameraComponent = InPrisoner->GetCameraComponent();
    CharacterData = InPrisoner->TweakeableData;
}

// Called every frame
void UInteractActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!MyPrisoner || !MyPrisoner->IsLocallyControlled() || !CanExecuteAction()) return;

    FCollisionQueryParams RV_TraceParams(FName(TEXT("RV_Trace")), true, MyPrisoner);

    FHitResult RV_Hit;
    FVector Start = CameraComponent->GetComponentLocation();
    FVector End = Start + (CameraComponent->GetForwardVector() * CharacterData->InteractRange);

    bool bHit = GetWorld()->LineTraceSingleByChannel(RV_Hit, Start, End, ECC_Camera, RV_TraceParams);

    if (bHit && RV_Hit.GetActor())
    {
        AInteractableObjects* IObj = Cast<AInteractableObjects>(RV_Hit.GetActor());

        // Masquer le widget de l'objet précédent, si nécessaire
        if (LastInteractableActor.IsValid())
        {
            AInteractableObjects* LastIObj = Cast<AInteractableObjects>(LastInteractableActor);
            if (LastIObj)
            {
                LastIObj->WidgetVisibility(false); // Cacher l'ancien widget
            }
        }

        // Mettre à jour l'acteur actuellement survolé
        LastInteractableActor = RV_Hit.GetActor();

        // Vérifier si l'acteur est interactable
        if (RV_Hit.GetActor()->Implements<UIInteractable>())
        {
            if (IIInteractable::Execute_CanInteract(RV_Hit.GetActor(), MyPrisoner))
            {
                if (IObj)
                    IObj->WidgetVisibility(true); // Afficher le widget pour l'objet actuel

                // Diffuser des informations sur l'objet survolé
                OnHoverInteractable.Broadcast(true, IIInteractable::Execute_GetInteractionName(RV_Hit.GetActor()), IIInteractable::Execute_GetTypeInteractionName(RV_Hit.GetActor()));
            }
            else
            {
                if (IObj)
                    IObj->WidgetVisibility(false); // Cacher le widget si l'objet ne peut pas être interagi

                OnHoverInteractable.Broadcast(false, "", "");
            }
        }
        else
        {
            if (IObj)
                IObj->WidgetVisibility(false); // Cacher le widget si l'objet n'est pas interactif

            OnHoverInteractable.Broadcast(false, "", "");
        }
    }
    else
    {
        // Si aucun objet interactif n'est détecté, masquer le widget
        if (LastInteractableActor.IsValid())
        {
            AInteractableObjects* LastIObj = Cast<AInteractableObjects>(LastInteractableActor);
            if (LastIObj)
            {
                LastIObj->WidgetVisibility(false); // Cacher le widget de l'objet précédent
            }
        }

        LastInteractableActor = nullptr;  // Réinitialiser l'acteur survolé
        OnHoverInteractable.Broadcast(false, "", "");
    }
}

