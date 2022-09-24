#include "RGX_InteractComponent.h"
#include "RegicideX/Interfaces/RGX_InteractInterface.h"
#include "DrawDebugHelpers.h"				// ::DrawDebugLine
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "RegicideX/UI/RGX_PlayerInteract.h"

DEFINE_LOG_CATEGORY(LogInteract);

URGX_InteractComponent::URGX_InteractComponent()
{
	// Because we want the tick to be active for these type of components
	PrimaryComponentTick.bCanEverTick = true;
}

void URGX_InteractComponent::BeginPlay()
{
	Super::BeginPlay();

	HideInteractWidget();
}

void URGX_InteractComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Test a sphere with objects

	AActor* Owner = GetOwner();

	const FVector OwnerLocation = Owner->GetActorLocation();
	const FVector OverlapLocation = OwnerLocation - FVector(0.0f, 0.0f, ZOffset);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(InteractableObjectType);

	//UClass* SeekClass = IRGX_InteractInterface::StaticClass();

	TArray<AActor*> IgnoreActors;

	TArray<AActor*> OutActors;

	bool bHitAnything = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), OverlapLocation, Radius, TraceObjectTypes, nullptr, IgnoreActors, OutActors);
	//::DrawDebugSphere(GetWorld(), OverlapLocation, Radius, 24.0f, FColor::Red);

	TWeakObjectPtr<AActor> NewActor = nullptr;
	IRGX_InteractInterface* NewInteract = nullptr;

	for (AActor* HitActor : OutActors)
	{
		//IRGX_InteractInterface* Interactable = Cast<IRGX_InteractInterface>(HitActor);
		if (HitActor->Implements<URGX_InteractInterface>())
		{
			bool bCanInteract = IRGX_InteractInterface::Execute_CanBeInteractedWith(HitActor, Owner);
			UE_LOG(LogTemp, Warning, TEXT("Can Be Interacted With: %s"), bCanInteract ? TEXT("TRUE") : TEXT("FALSE"));
			if (bCanInteract == true)
			{
				NewActor = HitActor;
				//NewInteract = Interactable;
				break;
			}
		}
	}

	// Did we change anything?
	if (NewActor == CurrentActor)
	{
		if (NewActor == nullptr)
		{
			HideInteractWidget();
		}

		return;
	}

	// Notify the old actor, we no longer want to interact with him
	if (CurrentActor.IsValid())
	{
		if (CurrentActor->Implements<URGX_InteractInterface>())
		{
			IRGX_InteractInterface::Execute_StopCanInteract(CurrentActor.Get(), Owner);
			UE_LOG(LogTemp, Warning, TEXT("Stop Can Interact"));
		}

		CurrentActor = nullptr;
	}

	// If we have something to interact with, notify it to the actor
	if (NewActor.IsValid())
	{
		CurrentActor = NewActor;
		IRGX_InteractInterface::Execute_StartCanInteract(NewActor.Get(), Owner);
		UE_LOG(LogTemp, Warning, TEXT("Start Can Interact"));
	}
}

void URGX_InteractComponent::TryToInteract()
{
	// If CurrentActor is null, CurrentInteract will return null
	if (CurrentActor == nullptr) return;

	if (CurrentActor->Implements<URGX_InteractInterface>())
	{
		//UE_LOG(LogInteract, Display, TEXT("Interacting with actor %s"), *CurrentActor->GetName());
		IRGX_InteractInterface::Execute_Interact(CurrentActor.Get(), GetOwner());
	}
}

void URGX_InteractComponent::ShowInteractWidget()
{
	InteractWidgetComponent->SetVisibility(true);
}

void URGX_InteractComponent::HideInteractWidget()
{
	InteractWidgetComponent->SetVisibility(false);
}

void URGX_InteractComponent::SetInteractionText(const FText& InteractionText)
{
	URGX_PlayerInteract* PlayerInteractWidget = Cast<URGX_PlayerInteract>(InteractWidgetComponent->GetWidget());
	PlayerInteractWidget->InteractionText = InteractionText;
}
