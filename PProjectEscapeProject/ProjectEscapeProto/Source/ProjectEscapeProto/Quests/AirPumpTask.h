#pragma once

#include "CoreMinimal.h"
#include "TaskBase.h"
#include "AirPumpTask.generated.h"

class ATaskSlot;
class APrisoner;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskTimerExpired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBalloonValidated, int32, BalloonsValidated);

/**
 * Classe AirPumpTask
 * Gère la tâche où deux joueurs utilisent des pompes pour gonfler des ballons.
 */
UCLASS()
class PROJECTESCAPEPROTO_API AAirPumpTask : public ATaskBase
{
    GENERATED_BODY()

public:
    // Constructeur par défaut
    AAirPumpTask();

protected:
    // Déclenché au début du jeu
    virtual void BeginPlay() override;

    // Fonction appelée à chaque frame
    virtual void Tick(float DeltaTime) override;

    // Met à jour la pression de la jauge
    void UpdatePressureGauge(float DeltaTime);

    // Valide le ballon si la pression reste stable dans la zone verte
    void ValidateBalloonProgress(float DeltaTime);

    // Gère l'apparition d'un nouveau ballon
    UFUNCTION(BlueprintCallable, Category = "Task")
    void SpawnNewBalloon();

    // Termine la tâche lorsque le temps est écoulé
    void EndTask();

public:
    // Enregistre une entrée utilisateur (barre espace)
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void RegisterPlayerInput(APrisoner* Player);

    // Mise à jour visuelle de la pression (implémentable en Blueprint)
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void UpdatePressureGaugeVisual(float CurrentPressure);

    // Démarre le timer global
    UFUNCTION(BlueprintCallable, Category = "Task")
    void StartTaskTimer(float TotalTime);

    // Déclenche un événement quand le timer expire
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTaskTimerExpired OnTimerExpired;

    // Déclenche un événement lorsqu'un ballon est validé
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBalloonValidated OnBalloonValidated;

    // --- Paramètres Configurables depuis Blueprint ---

    // Référence au premier slot
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
    ATaskSlot* PumpSlot1;

    // Référence au second slot
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
    ATaskSlot* PumpSlot2;

    // Valeur actuelle de la jauge de pression
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balloon")
    float PressureGaugeValue;

    // Limite inférieure de la zone verte
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balloon")
    float GreenZoneMin;

    // Limite supérieure de la zone verte
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balloon")
    float GreenZoneMax;

    /** Temps passé dans la zone verte (en secondes) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Balloon", meta = (AllowPrivateAccess = "true"))
    float TimeInGreenZone;

    // Temps nécessaire dans la zone verte pour valider un ballon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balloon")
    float ValidationTime;

    // Puissance d'incrémentation à chaque frappe
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float PumpPowerIncrement;

    // Temps restant pour compléter la tâche
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Timer")
    float TimeRemaining;

    // Nombre total de ballons validés
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Balloon")
    int32 BalloonsValidated;
};
