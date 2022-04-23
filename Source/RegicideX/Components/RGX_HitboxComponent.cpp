#include "RGX_HitboxComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

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
	SetCollisionProfileName("HitboxPreset");
}

void URGX_HitboxComponent::DeactivateHitbox()
{
	SetCollisionProfileName("HitboxPreset");
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void URGX_HitboxComponent::ApplyEffects(AActor* OtherActor)
{
	if (ensureMsgf(EffectToApply.Get(), TEXT("URX_HitboxComponent::ApplyEffects No valid effect to apply")))
	{
		// Try to get owner ASC
		UAbilitySystemComponent* ApplierASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
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
	AActor* OwnerActor = GetOwner();
	ETeamAttitude::Type Attitude = FGenericTeamId::GetAttitude(OwnerActor, OtherActor);
	if (Attitude == TeamToApply)
	{
		ApplyEffects(OtherActor);
	}
}
