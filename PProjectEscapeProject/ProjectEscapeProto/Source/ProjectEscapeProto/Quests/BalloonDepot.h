#pragma once

#include "CoreMinimal.h"
#include "ProjectEscapeProto/Objects/Items/Item.h"
#include "BalloonDepot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBalloonFinished, bool, WithSuccess);

UCLASS()
class PROJECTESCAPEPROTO_API ABalloonDepot : public AItem
{
    GENERATED_BODY()

protected:
    // Implémentations des fonctions d'interaction
    virtual bool CanInteract_Implementation(AActor* Actor) override;
    virtual bool TryInteract_Implementation(AActor* Actor) override;
    virtual bool TryStopInteract_Implementation(AActor* Actor) override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;

    // Mise à jour UI côté client
    UFUNCTION(BlueprintImplementableEvent, Category = "Balloon")
    void UpdateBalloonDepotUI(float BalloonPercent);

    UFUNCTION(BlueprintImplementableEvent, Category = "Balloon")
    void BP_OnBalloonFinished(bool WithSuccess);

    UFUNCTION(BlueprintImplementableEvent, Category = "Balloon")
    void BP_OnBalloonStarted();

    UFUNCTION(BlueprintImplementableEvent, Category = "Balloon")
    void BP_OnCannotInteract();

    // Fonction appelée lors de la réplication de CurrentDelay
    UFUNCTION()
    void OnRep_CurrentDelay();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Balloon")
    float GetBalloonHealth() const { return CurrentDelay / DelayBetweenUse; }

    // Expose le matériau dynamique pour les Blueprints
    UFUNCTION(BlueprintCallable, Category = "Balloon")
    UMaterialInstanceDynamic* GetDynamicMaterial() const { return MaterialInstance; }

public:
    // Événement lié à la fin de l'interaction
    FOnBalloonFinished OnBalloonFinished;

protected:
    // Variables
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Balloon")
    int BalloonPoints = 400;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Balloon")
    float DelayBetweenUse = 2.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentDelay, Category = "Balloon")
    float CurrentDelay = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Balloon")
    int PlayerInteractingId = -1;

    // Matériau dynamique (accessible par les Blueprints)
    UPROPERTY(BlueprintReadOnly, Category = "Balloon")
    UMaterialInstanceDynamic* MaterialInstance;

    // Matériau par défaut
    UPROPERTY(EditDefaultsOnly, Category = "Balloon")
    UMaterialInterface* DefaultMaterial; // Référence à "MI_UI_Interact"
};
