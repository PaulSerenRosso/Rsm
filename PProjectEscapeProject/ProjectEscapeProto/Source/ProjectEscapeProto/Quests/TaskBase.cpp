#include "TaskBase.h"
#include "TaskSlot.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

// Log Category spécifique pour cette classe
DEFINE_LOG_CATEGORY_STATIC(LogTaskBase, Log, All);

// Constructeur
ATaskBase::ATaskBase()
{
	PrimaryActorTick.bCanEverTick = false; // Tick désactivé par défaut
	TaskState = ETaskState::NotStarted; // Par défaut, tâche non commencée
	AActor::SetReplicateMovement(true); // Active la réplication du mouvement
}

void ATaskBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeTaskSlots();

	// Désactiver Tick si aucune mise à jour frame par frame n'est nécessaire
	if (TaskState == ETaskState::NotStarted)
	{
		SetActorTickEnabled(false);
	}
	IncompleteInteraction = true;
}

void ATaskBase::InitializeTaskSlots()
{
	// Vide le tableau pour éviter tout doublon
	TaskSlots.Empty();

	// Parcours des Child Actors de l'acteur courant
	TArray<UChildActorComponent*> ChildActors;
	GetComponents<UChildActorComponent>(ChildActors);

	for (UChildActorComponent* ChildActor : ChildActors)
	{
		if (ChildActor)
		{
			// Vérifie si le Child Actor est une instance de TaskSlot
			ATaskSlot* TaskSlot = Cast<ATaskSlot>(ChildActor->GetChildActor());
			if (TaskSlot)
			{
				TaskSlot->OnEndSlot.AddDynamic(this, &ATaskBase::CompleteTask);
				TaskSlots.Add(TaskSlot);
				UE_LOG(LogTaskBase, Log, TEXT("TaskSlot %s initialisé dans %s."), *TaskSlot->GetName(), *GetName());
			}
		}
	}

	UE_LOG(LogTaskBase, Log, TEXT("Total des TaskSlots initialisés dans %s : %d."), *GetName(), TaskSlots.Num());
}


void ATaskBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATaskBase::StartTask()
{
	if (TaskState == ETaskState::NotStarted)
	{
		TaskState = ETaskState::InProgress;
		UE_LOG(LogTaskBase, Log, TEXT("Task %s started."), *GetName());

		// for (ATaskSlot* Slot : TaskSlots)
		// {
		// 	if (Slot)
		// 	{
		// 		Slot->bCanExit = false;
		// 	}
		// }

		//SetActorTickEnabled(true); // Active le Tick si nécessaire
	}
}

void ATaskBase::CompleteTask()
{
	TaskState = ETaskState::Completed;
	UE_LOG(LogTaskBase, Log, TEXT("Task %s completed."), *GetName());

	// Libère tous les joueurs des slots
	for (ATaskSlot* Slot : TaskSlots)
	{
		if (Slot && Slot->OccupyingPrisoner)
		{
			Slot->ServerNotExitSlot();
			Slot->SetCanBeInteracted(false);
		}
	}

	SetActorTickEnabled(false);
	IncompleteInteraction = false;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		OnTaskCompleted.Broadcast();
	}, 0.001f, false);
}

bool ATaskBase::CanPlayerEnterSlot(APrisoner* Player) const
{
	return TaskState == ETaskState::InProgress && Player != nullptr;
}

void ATaskBase::DeactivateInteraction()
{
	Super::DeactivateInteraction();
	for (auto Slot : TaskSlots)
	{
		if (!Slot) continue;
		Slot->DeactivateInteraction();
	}
}

void ATaskBase::PlayerEnteredSlot(APrisoner* Player, ATaskSlot* Slot)
{
	if (!CanPlayerEnterSlot(Player))
	{
		UE_LOG(LogTaskBase, Warning, TEXT("Player %s cannot enter slot."), *Player->GetName());
		return;
	}

	UE_LOG(LogTaskBase, Log, TEXT("Player %s entered slot in Task %s."), *Player->GetName(), *GetName());
	CurrentPlayer = Player;

	bool AllSlotsOccupied = true;
	for (ATaskSlot* TaskSlot : TaskSlots)
	{
		if (TaskSlot && !TaskSlot->OccupyingPrisoner)
		{
			AllSlotsOccupied = false;
			break;
		}
	}

	if (AllSlotsOccupied)
	{
		UE_LOG(LogTaskBase, Log, TEXT("All slots in Task %s are occupied!"), *GetName());
		CompleteTask();
	}
}

void ATaskBase::PlayerExitedSlot(APrisoner* Player)
{
	if (CurrentPlayer == Player)
	{
		UE_LOG(LogTaskBase, Log, TEXT("Player %s exited slot in Task %s."), *Player->GetName(), *GetName());
		CurrentPlayer = nullptr;
	}
}
