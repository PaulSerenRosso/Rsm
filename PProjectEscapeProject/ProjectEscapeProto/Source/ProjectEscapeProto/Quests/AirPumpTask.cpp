#include "AirPumpTask.h"
#include "TaskSlot.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "Engine/World.h"
#include "TimerManager.h"

AAirPumpTask::AAirPumpTask()
{
    PrimaryActorTick.bCanEverTick = true; // Tick activé pour gérer les mises à jour du compteur
    PressureGaugeValue = 0.0f;
    GreenZoneMin = 40.0f;
    GreenZoneMax = 60.0f;
    ValidationTime = 2.0f;
    PumpPowerIncrement = 1.0f;
    TimeRemaining = 60.0f;
    BalloonsValidated = 0;
    TimeInGreenZone = 0.0f;
}

void AAirPumpTask::BeginPlay()
{
    Super::BeginPlay();

    // Initialisation ou vérification des slots
    if (!PumpSlot1 || !PumpSlot2)
    {
        UE_LOG(LogTemp, Warning, TEXT("Pump slots are not properly assigned in %s. Check the Blueprint setup."), *GetName());
    }
}

void AAirPumpTask::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TimeRemaining > 0.0f)
    {
        // Diminue le temps restant
        TimeRemaining = FMath::Max(TimeRemaining - DeltaTime, 0.0f);

        // Mises à jour logiques
        UpdatePressureGauge(DeltaTime);
        ValidateBalloonProgress(DeltaTime);

        // Vérifie si le temps est écoulé
        if (TimeRemaining <= 0.0f)
        {
            EndTask();
        }
    }
}

void AAirPumpTask::StartTaskTimer(float TotalTime)
{
    // Initialise le temps restant
    TimeRemaining = TotalTime;

    // Ajoutez des logs pour confirmer que la méthode est appelée
    UE_LOG(LogTemp, Log, TEXT("Task timer started with %f seconds."), TotalTime);
}

void AAirPumpTask::RegisterPlayerInput(APrisoner* Player)
{
    // Vérifie que le joueur est dans l'un des deux slots
    int LocalPlayerId = Player->GetPlayerState()->GetPlayerId();
    if (Player && (LocalPlayerId == PumpSlot1->OccupyingPrisoner || LocalPlayerId == PumpSlot2->OccupyingPrisoner))
    {
        // Augmente la pression en fonction de l'entrée utilisateur
        PressureGaugeValue = FMath::Clamp(PressureGaugeValue + PumpPowerIncrement, 0.0f, 100.0f);

        // Mise à jour visuelle
        if (PressureGaugeValue >= 0.0f && PressureGaugeValue <= 100.0f)
        {
            UpdatePressureGaugeVisual(PressureGaugeValue);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Player input ignored as player is not in a valid pump slot."));
    }
}

void AAirPumpTask::UpdatePressureGauge(float DeltaTime)
{
    // Réduction progressive de la pression pour ajouter de la difficulté
    PressureGaugeValue = FMath::Clamp(PressureGaugeValue - (DeltaTime * 5.0f), 0.0f, 100.0f);

    // Mise à jour visuelle via Blueprint
    UpdatePressureGaugeVisual(PressureGaugeValue);
}

void AAirPumpTask::ValidateBalloonProgress(float DeltaTime)
{
    // Vérifie si la pression est dans la zone verte
    if (PressureGaugeValue >= GreenZoneMin && PressureGaugeValue <= GreenZoneMax)
    {
        TimeInGreenZone += DeltaTime;

        // Si le joueur reste dans la zone verte assez longtemps, valide le ballon
        if (TimeInGreenZone >= ValidationTime)
        {
            BalloonsValidated++;
            OnBalloonValidated.Broadcast(BalloonsValidated);
            SpawnNewBalloon();
            TimeInGreenZone = 0.0f; // Réinitialise le timer pour le prochain ballon
        }
    }
    else
    {
        TimeInGreenZone = 0.0f; // Réinitialise si l'aiguille sort de la zone verte
    }
}

void AAirPumpTask::SpawnNewBalloon()
{
    // Ajoute un log pour débogage
    UE_LOG(LogTemp, Log, TEXT("New balloon spawned! Total balloons validated: %d"), BalloonsValidated);

    // Logique d'apparition d'un nouveau ballon (ex: reset d'un composant visuel ou création)
    // Peut être implémentée en Blueprint avec des particules, sons, etc.
}

void AAirPumpTask::EndTask()
{
    // Diffuse un événement indiquant que la tâche est terminée
    OnTimerExpired.Broadcast();

    // Log pour indiquer que la tâche est terminée
    UE_LOG(LogTemp, Log, TEXT("Task ended. Total balloons validated: %d"), BalloonsValidated);

    // Logique supplémentaire (désactivation des inputs, affichage du score, etc.)
}
