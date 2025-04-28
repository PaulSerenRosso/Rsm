#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TaskSlot.h"
#include "TaskBase.generated.h"

class APrisoner;

/** Enumération des états possibles pour une tâche */
UENUM(BlueprintType)
enum class ETaskState : uint8
{
    NotStarted UMETA(DisplayName = "Not Started"),
    InProgress UMETA(DisplayName = "In Progress"),
    Completed UMETA(DisplayName = "Completed")
};

/** Delegate dynamique appelé lorsqu'une tâche est terminée */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskCompleted, ATaskBase*, CompletedTask);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskCompleted);

/**
 * Classe ATaskBase
 * Représente une tâche dans le jeu, comprenant des slots et une gestion d'état.
 */
UCLASS()
class PROJECTESCAPEPROTO_API ATaskBase : public ASpawnActor
{
    GENERATED_BODY()

public:
    /** Constructeur par défaut */
    ATaskBase();

protected:
    /** Appelé au début du jeu ou lors de l'initialisation */
    virtual void BeginPlay() override;
    
    /** Initialise les TaskSlots depuis les Child Actors */
    void InitializeTaskSlots();

public:
    /** Appelé chaque frame (si activé) */
    virtual void Tick(float DeltaTime) override;

    /** Démarre la tâche (change l'état en "In Progress") */
    UFUNCTION(BlueprintCallable, Category = "Task")
    void StartTask();

    /** Termine la tâche (change l'état en "Completed") et déclenche le delegate */
    UFUNCTION(BlueprintCallable, Category = "Task")
    void CompleteTask();

    /**
     * Vérifie si un joueur peut entrer dans un slot
     * @param Player Référence au joueur
     * @return True si le joueur peut entrer
     */
    UFUNCTION(BlueprintCallable, Category = "Task")
    bool CanPlayerEnterSlot(APrisoner* Player) const;

    virtual void DeactivateInteraction() override;

    /**
     * Gère l'entrée d'un joueur dans un slot
     * @param Player Référence au joueur
     * @param Slot Le slot où le joueur entre
     */
    UFUNCTION(BlueprintCallable, Category = "Task")
    void PlayerEnteredSlot(APrisoner* Player, ATaskSlot* Slot);

    /**
     * Gère la sortie d'un joueur d'un slot
     * @param Player Référence au joueur qui sort
     */
    UFUNCTION(BlueprintCallable, Category = "Task")
    void PlayerExitedSlot(APrisoner* Player);

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Task|Delegates")
    FOnTaskCompleted OnTaskCompleted;
    
    // **Propriétés**
protected:
    /** État actuel de la tâche */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task|State")
    ETaskState TaskState;

    /** Liste des slots inclus dans cette tâche */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Task|Slots", meta = (AllowPrivateAccess = "true"))
    TArray<ATaskSlot*> TaskSlots;

    /** Joueur actuellement associé à cette tâche (peut être nul) */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Task|Current")
    APrisoner* CurrentPlayer;

    /** Distance horizontale entre les TaskSlots (modifiable dans Blueprint) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task|Setup")
    float SlotSpacing = 200.0f;

    /** Delegate appelé lorsque la tâche est complétée */

};
