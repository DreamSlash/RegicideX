#include "RGX_HitboxContainerComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/ChildActorComponent.h"
#include "GenericTeamAgentInterface.h"

URGX_HitboxContainerComponent::URGX_HitboxContainerComponent()
{

}

void URGX_HitboxContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get references to all children colliders (colliders that represent the hitbox)
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		UShapeComponent* ShapeComponent = Cast<UShapeComponent>(Child);

		if (ShapeComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Collider Added to Hitbox\n"));
			Shapes.Add(ShapeComponent);
		}
	}

	for (UShapeComponent* shape : Shapes)
	{
		shape->OnComponentBeginOverlap.AddDynamic(this, &URGX_HitboxContainerComponent::OnComponentOverlap);
	}

	if (bStartActive)
	{
		ActivateHitbox();
	}
	else
	{
		DeactivateHitbox();
	}
}

void URGX_HitboxContainerComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (UShapeComponent* shape : Shapes)
	{
		shape->OnComponentBeginOverlap.RemoveDynamic(this, &URGX_HitboxContainerComponent::OnComponentOverlap);
	}
}

void URGX_HitboxContainerComponent::ActivateHitbox()
{
	for (UShapeComponent* shape : Shapes)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		shape->SetCollisionProfileName("Dodgeable");
	}
}

void URGX_HitboxContainerComponent::DeactivateHitbox()
{
	for (UShapeComponent* shape : Shapes)
	{
		shape->SetCollisionProfileName("Dodgeable");
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ActorsHit.Empty();
}

void URGX_HitboxContainerComponent::ApplyEffects(AActor* OtherActor)
{
	for (TSubclassOf<UGameplayEffect>& Effect : EffectsToApply)
	{
		if (!ensureMsgf(Effect.Get(), TEXT("URGX_HitboxComponent::ApplyEffects No valid effect to apply")))
		{
			continue;
		}

		USceneComponent* Parent = GetAttachParent();
		AActor* OwnerActor = Parent->GetAttachmentRootActor();

		if (!OwnerActor)
		{
			OwnerActor = GetOwner();
		}

		// Try to get owner ASC
		UAbilitySystemComponent* ApplierASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);
		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
		// If not fallback to target
		if (!ApplierASC)
		{
			ApplierASC = TargetASC;
		}

		// Only apply if ASC valid
		if (ApplierASC && TargetASC)
		{
			ApplierASC->ApplyGameplayEffectToTarget(Effect->GetDefaultObject<UGameplayEffect>(), TargetASC, 1, ApplierASC->MakeEffectContext());
		}
	}
}

void URGX_HitboxContainerComponent::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (AActor* Hit : ActorsHit)
	{
		// An actor cannot be hit more than once by the same hitbox activation
		if (OtherActor == Hit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Same Actor\n"));
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Overlap\n"));
	USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();

	if (!OwnerActor)
	{
		OwnerActor = GetOwner();
	}

	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(OwnerActor);

	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(OwnerActor, OtherActor);
	if (Attitude == TeamToApply)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitbox Overlap"));
		ActorsHit.Add(OtherActor);
		ApplyEffects(OtherActor);
	}
}