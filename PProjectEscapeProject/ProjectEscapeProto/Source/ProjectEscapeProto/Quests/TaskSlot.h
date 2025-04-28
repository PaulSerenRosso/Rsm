#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/Objects/InteractableObjects.h"
#include "TimerManager.h"
#include "Animation/AnimationAsset.h"
#include "GameFramework/Actor.h"
#include "Components/ChildActorComponent.h"
#include "TaskSlot.generated.h"

class UInputAction;
class UInputMappingContext;
class APrisonerController;
class APrisoner;
class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerPressInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnterSlot);

UCLASS(Blueprintable, BlueprintType)
class PROJECTESCAPEPROTO_API ATaskSlot : public AInteractableObjects
{
    GENERATED_BODY()

public:
    // Constructeur par défaut
    ATaskSlot();
    
    /** Vérifie si un joueur peut interagir avec ce slot */
    virtual bool CanInteract_Implementation(AActor* Actor) override;

    /** Gère l'interaction lorsqu'un joueur essaye d'utiliser ce slot */
    virtual bool TryInteract_Implementation(AActor* Actor) override;

    /** Gestion de l'entrée dans le slot (appelée côté serveur) */
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Slot")
    void ServerEnterSlot(APrisoner* Prisoner);

    /** Synchronisation sur tous les clients lors de l'entrée dans le slot */
    UFUNCTION(NetMulticast, Reliable, Category = "Slot")
    void MulticastEnterSlot(APrisoner* Prisoner);

    /** Gestion de la sortie du slot (appelée côté serveur) */
    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Slot")
    void ServerExitSlot();


    /** Gestion de la sortie du slot (appelée côté serveur) */
    UFUNCTION(Server, Reliable, WithValidation, Category = "Slot")
    void SERVER_InteractOnServer();
    
    /** Synchronisation sur tous les clients lors de la sortie du slot */
    UFUNCTION(NetMulticast, Reliable, Category = "Slot")
    void MulticastExitSlot();
    UFUNCTION(Category = "Slot")
    void ExitSlot();
    
    /** Active la possibilité pour un joueur de sortir du slot après un délai */
    UFUNCTION(BlueprintCallable, Category = "Slot")
    void EnableExit();

    UFUNCTION()
    void HandlePressInteract(APrisonerController* PC);
    
    UFUNCTION(BlueprintCallable, Category = "Slot")
    void OnPlayerPressInteract();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Slot")
    void OnPlayerInteract();
    UFUNCTION(BlueprintCallable)
    void ChangeFloatValue(float NewValue);
    UFUNCTION(Server, Reliable)
    void ServerChangeFloatValue(float NewValue);
    void ForceExitSlot(int PlayerId);
    void EndOfSlot();
    UFUNCTION(BlueprintImplementableEvent, Category = "Slot")
    void BP_EndOfSlot();

    void SetCanBeInteracted(bool Value) { IncompleteInteraction = Value; }
    void ServerNotExitSlot();

    
protected:
    /** Appelé au début du jeu ou lors de l'initialisation */
    virtual void BeginPlay() override;

    void StopInteract();

    bool IsMyself();

    UFUNCTION(BlueprintImplementableEvent, Category = "Slot")
    void OnEnterSlot(APrisoner* Prisoner);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Slot")
    void OnExitSlot(APrisoner* Prisoner);

    UFUNCTION()
    void ExitSlotWithIdParameter(int PlayerId);

    //void ForceExitSlot(int PlayerId);

private:
    /** Validation pour ServerEnterSlot */
    bool ServerEnterSlot_Validate(APrisoner* Prisoner);

    /** Validation pour ServerExitSlot */
    bool ServerExitSlot_Validate();

public:
    /** Composant pour définir la position où un joueur sera placé dans le slot */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
    USceneComponent* SlotPosition;

    /** Caméra attachée pour gérer les transitions de vue personnalisées */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    UChildActorComponent* CameraPivot;

    /** Animation pour entrer dans le slot en vue première personne */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot|Animations")
    UAnimationAsset* SlotAnim1P;

    /** Animation pour entrer dans le slot en vue troisième personne */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot|Animations")
    UAnimationAsset* SlotAnim3P;
    
    /** Référence au joueur actuellement dans le slot */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Slot")
   int OccupyingPrisoner = -1;

    /** Délai avant de permettre au joueur de sortir */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    float ExitDelay = 1.0f;

    /** Handle pour gérer le Timer utilisé pour le délai de sortie */
    FTimerHandle ExitDelayHandle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
    UInputMappingContext* MyTaskSlotInputMapping;

    FOnPlayerEnterSlot OnEndSlot;
    UPROPERTY(BlueprintAssignable, Category = "Slot")
    FOnPlayerPressInteract OnPlayerPressInteractBP;

protected:

    UPROPERTY(BlueprintAssignable, Category = "Slot")
    FOnPlayerEnterSlot OnPlayerEnterSlotBP;
    UPROPERTY(BlueprintAssignable, Category = "Slot")
    FOnPlayerEnterSlot OnPlayerExitSlotBP;


    UPROPERTY(BlueprintReadOnly, Replicated)
    float TaskValue = 0.0f;

};
