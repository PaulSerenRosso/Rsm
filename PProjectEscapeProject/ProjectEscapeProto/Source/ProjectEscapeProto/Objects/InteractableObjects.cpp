#include "InteractableObjects.h"
#include "ProjectEscapeProto/Characters/Prisoner.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectEscapeProto/Widgets/InteractableWidget.h"

AInteractableObjects::AInteractableObjects()
{
    PrimaryActorTick.bCanEverTick = true; // Active le Tick
    bReplicates = true;

    // Initialize InteractionRoot
    InteractionRoot = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionRoot"));

    // Initialize InteractionSphere
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    
    InteractionSphere->SetRelativeLocation(FVector::ZeroVector);
    InteractionSphere->SetSphereRadius(150.0f);
    InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
    

    // Initialize InteractionWidget
    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
   
    InteractionWidget->SetRelativeLocation(FVector(0, 0, 20));
    InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
    InteractionWidget->SetDrawSize(FVector2D(250.f, 110.f));
    InteractionWidget->SetVisibility(false);

    // Load Widget Class
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Blueprints/Widgets/WBP_Interactable"));
    if (WidgetClass.Succeeded())
    {
        InteractionWidget->SetWidgetClass(WidgetClass.Class);
    }
}

void AInteractableObjects::BeginPlay()
{
    Super::BeginPlay();
    SetReplicateMovement(true);
    InteractionRoot->AttachToComponent(ItemMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    InteractionSphere->AttachToComponent(InteractionRoot, FAttachmentTransformRules::KeepRelativeTransform);
    InteractionWidget->AttachToComponent(InteractionRoot, FAttachmentTransformRules::KeepRelativeTransform);
    
    InteractionRoot->SetRelativeLocation(FVector(InteractionRootLocation));
    InteractionSphere->SetRelativeLocation(FVector(0, 0, 20));
    InteractionWidget->SetRelativeLocation(FVector(0, 0, 20));
     if (InteractionWidget && InteractionWidget->GetUserWidgetObject())
     {
         UUserWidget* WidgetInstance = InteractionWidget->GetUserWidgetObject();
         if (UTextBlock* TextBlock = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName("InteractionText")))
         {
               TextBlock->SetText(FText::FromString(TypeInteractionName + InteractableName));
         }
     }
}

void AInteractableObjects::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool AInteractableObjects::CanInteract_Implementation(AActor* Actor)
{
    APrisoner* Prisoner = Cast<APrisoner>(Actor);
    return Prisoner != nullptr && IncompleteInteraction && CanBeUsed;
}

bool AInteractableObjects::TryInteract_Implementation(AActor* Actor)
{
    APrisoner* Prisoner = Cast<APrisoner>(Actor);
    if (IncompleteInteraction && Prisoner != nullptr)
    {
        OnInteract.Broadcast(Prisoner);
        return true;
    }
    return false;
}

FString AInteractableObjects::GetTypeInteractionName_Implementation()
{
    return FString("Use ");
}

void AInteractableObjects::WidgetVisibility(bool ShowWidget)
{
    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(ShowWidget);
    }
}
