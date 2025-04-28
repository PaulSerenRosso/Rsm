#include "BalloonDepot.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "ProjectEscapeProto/ActorComponents/InventorySystem.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "ProjectEscapeProto/Objects/Items/Balloon.h"
#include "ProjectEscapeProto/Utilities/FunctionsUtilities.h"

void ABalloonDepot::BeginPlay()
{
    Super::BeginPlay();

    if (DefaultMaterial && InteractionWidget)
    {
        // Crée une instance dynamique à partir du matériau par défaut
        MaterialInstance = UMaterialInstanceDynamic::Create(DefaultMaterial, this);
    }
}

bool ABalloonDepot::CanInteract_Implementation(AActor* Actor)
{
    APrisoner* Prisoner = Cast<APrisoner>(Actor);
    if (!Prisoner) return false;

    UInventorySystem* Inventory = Prisoner->Inventory;
    if (!Inventory) return false;

    AItem* Item = Inventory->FirstItemSlot;

    // Si l'objet tenu par le joueur n'est pas un ballon, appeler l'événement Blueprint
    if (!Item || !Cast<ABalloon>(Item))
    {
        BP_OnCannotInteract(); // Appelle l'événement Blueprint
        return false;
    }

    // Si les conditions sont remplies, interaction possible
    return (PlayerInteractingId == -1 || PlayerInteractingId == Prisoner->GetPlayerState()->GetPlayerId() && CanBeUsed);
}


bool ABalloonDepot::TryInteract_Implementation(AActor* Actor)
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TryInteract_Implementation"));

    // Activer le tick pour suivre la progression
    SetActorTickEnabled(true);
    CurrentDelay = 0.0f;
    PlayerInteractingId = Cast<APrisoner>(Actor)->GetPlayerState()->GetPlayerId();
    
    // Mise à jour immédiate du matériau
    if (MaterialInstance)
    {
        float ProgressValue = 1.0f - GetBalloonHealth();
        MaterialInstance->SetScalarParameterValue(FName("Progress"), ProgressValue);
    }

    // Notifier les Blueprints
    BP_OnBalloonStarted();
    return true;
}

bool ABalloonDepot::TryStopInteract_Implementation(AActor* Actor)
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TryStopInteract_Implementation"));
    SetActorTickEnabled(false);
    if (CurrentDelay >= DelayBetweenUse) return false;
    PlayerInteractingId = -1;

    BP_OnBalloonFinished(false);
    return true;
}

void ABalloonDepot::Tick(float DeltaSeconds)
{
    if (!HasAuthority()) return;
    if (PlayerInteractingId == -1) return;

    APrisoner* Prisoner = UFunctionsUtilities::Server_GetPlayerWithId(this, PlayerInteractingId);
    if (!Prisoner) return;

    AItem* Item = Prisoner->Inventory->FirstItemSlot;
    if (!Item)
    {
        PlayerInteractingId = -1;
        SetActorTickEnabled(false);
        return;
    }

    if (CurrentDelay < DelayBetweenUse)
    {
        CurrentDelay += DeltaSeconds;
        if (CurrentDelay > DelayBetweenUse)
            CurrentDelay = DelayBetweenUse;

        if (MaterialInstance)
        {
            float ProgressValue = 1.0f - GetBalloonHealth();
            MaterialInstance->SetScalarParameterValue(FName("Progress"), ProgressValue);
        }
    }
    else
    {
        SetActorTickEnabled(false);

        Prisoner->Inventory->TryRemoveItem(Item, Prisoner, true);
        ABalloon* Balloon = Cast<ABalloon>(Item);
        if (!Balloon) return;
        Balloon->ValidateBalloon();
        OnBalloonFinished.Broadcast(true);
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Item]()
        {
            Item->Destroy();
        }, 0.001f, false);
        CanBeUsed = false;
        BP_OnBalloonFinished(true);
        Prisoner->AddPointToPlayer(BalloonPoints);
    }
}

void ABalloonDepot::OnRep_CurrentDelay()
{
    if (MaterialInstance)
    {
        float ProgressValue = 1.0f - GetBalloonHealth();
        MaterialInstance->SetScalarParameterValue(FName("Progress"), ProgressValue);
    }
}

void ABalloonDepot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ABalloonDepot, PlayerInteractingId);
    DOREPLIFETIME(ABalloonDepot, DelayBetweenUse);
    DOREPLIFETIME(ABalloonDepot, CurrentDelay);
}
