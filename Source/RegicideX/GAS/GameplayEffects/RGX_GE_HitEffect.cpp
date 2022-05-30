#include "RGX_GE_HitEffect.h"

URGX_HitEffect::URGX_HitEffect()
{
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Combat.TakeDamage")));
	DurationPolicy = EGameplayEffectDurationType::Infinite;
}