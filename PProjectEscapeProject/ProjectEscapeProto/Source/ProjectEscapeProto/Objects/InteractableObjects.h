#pragma once

#include "CoreMinimal.h"
#include "SpawnActor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectEscapeProto/Interfaces/IInteractable.h"
#include "InteractableObjects.generated.h"

class APrisoner;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, APrisoner*, Prisoner);

UCLASS(Blueprintable)
class PROJECTESCAPEPROTO_API AInteractableObjects : public ASpawnActor, public IIInteractable
{
	GENERATED_BODY()

public:
	
    AInteractableObjects();

protected:
	
    virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Fonction qui sera appelée lorsqu'on interagit avec le bouton
	virtual bool TryInteract_Implementation(AActor* Actor) override;

	// Fonction qui vérifie si un acteur peut interagir avec le bouton
	virtual bool CanInteract_Implementation(AActor* Actor) override;

	virtual FString GetTypeInteractionName_Implementation() override;
	void WidgetVisibility(bool ShowWidget);
	
    // Interaction name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FString InteractableName = "Interactable Object";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FString TypeInteractionName = "Use ";

	// Déclare un événement quand interaction
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteract OnInteract;
	
	// INTERACTION ROOT
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    USceneComponent* InteractionRoot;

	// INTERACTION ROOT LOCATION
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FVector InteractionRootLocation;
	
	// Composant pour la détéction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	USphereComponent* InteractionSphere;

    // Widget d'interaction
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    UWidgetComponent* InteractionWidget;

};

