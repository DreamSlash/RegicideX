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
	::DrawDebugSphere(GetWorld(), OverlapLocation, Radius, 24.0f, FColor::Red);

	TWeakObjectPtr<AActor> NewActor = nullptr;
	IRGX_InteractInterface* NewInteract = nullptr;

	for (AActor* HitActor : OutActors)
	{
		IRGX_InteractInterface* Interactable = Cast<IRGX_InteractInterface>(HitActor);
		if (Interactable)
		{
			bool bCanInteract = Interactable->CanBeInteractedWith(Owner);
			if (bCanInteract == true)
			{
				NewActor = HitActor;
				NewInteract = Interactable;
				break;
			}
		}
	}

	if (CurrentActor == nullptr && bHitAnything == false)
	{
		HideInteractWidget();
		return;
	}

	// Did we change anything?
	if (NewActor == CurrentActor)
	{
		return;
	}

	// Notify the old actor, we no longer want to interact with him
	if (CurrentActor.IsValid())
	{
		IRGX_InteractInterface* CurrentInteract = Cast<IRGX_InteractInterface>(CurrentActor);
		CurrentInteract->StopCanInteract(Owner);
		CurrentActor = nullptr;
	}

	// If we have something to interact with, notify it to the actor
	if (NewInteract)
	{
		CurrentActor = NewActor;
		NewInteract->StartCanInteract(Owner);
	}
}

void URGX_InteractComponent::TryToInteract()
{
	// If CurrentActor is null, CurrentInteract will return null
	IRGX_InteractInterface* CurrentInteract = Cast<IRGX_InteractInterface>(CurrentActor);
	if (CurrentInteract)
	{
		//UE_LOG(LogInteract, Display, TEXT("Interacting with actor %s"), *CurrentActor->GetName());
		CurrentInteract->Interact(GetOwner());
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
