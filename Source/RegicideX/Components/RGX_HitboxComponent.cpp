#include "RGX_HitboxComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/ChildActorComponent.h"
#include "GenericTeamAgentInterface.h"

URGX_HitboxComponent::URGX_HitboxComponent()
{

}

void URGX_HitboxComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &URGX_HitboxComponent::OnComponentOverlap);

	if (bStartActive)
	{
		ActivateHitbox();
	}
	else
	{
		DeactivateHitbox();
	}
}

void URGX_HitboxComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OnComponentBeginOverlap.RemoveDynamic(this, &URGX_HitboxComponent::OnComponentOverlap);
}

void URGX_HitboxComponent::ActivateHitbox()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionProfileName("Dodgeable");
}

void URGX_HitboxComponent::DeactivateHitbox()
{
	SetCollisionProfileName("Dodgeable");
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void URGX_HitboxComponent::ApplyEffects(AActor* OtherActor)
{
	if (ensureMsgf(EffectToApply.Get(), TEXT("URGX_HitboxComponent::ApplyEffects No valid effect to apply")))
	{
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
			ApplierASC->ApplyGameplayEffectToTarget(EffectToApply->GetDefaultObject<UGameplayEffect>(), TargetASC, 1, ApplierASC->MakeEffectContext());
		}
	}
}

void URGX_HitboxComponent::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap\n"));
	USceneComponent* Parent = GetAttachParent();
	AActor* OwnerActor = Parent->GetAttachmentRootActor();

	if (!OwnerActor)
	{
		OwnerActor = GetOwner();
	}

	/*
	if (OwnerActor)
	{
		FString ActorName = OwnerActor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO ATTACHED PARENT\n"));
	}
	*/

	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(OwnerActor);

	/*
	if (TeamAgentA)
	{
		uint8 teamID = TeamAgentA->GetGenericTeamId().GetId();
		UE_LOG(LogTemp, Warning, TEXT("TEAM ID: %d\n"), teamID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TeamAgentA is nullptr\n"));
	}
	*/
	
	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(OwnerActor, OtherActor);
	if (Attitude == TeamToApply)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitbox Overlap"));
		ApplyEffects(OtherActor);
	}
}