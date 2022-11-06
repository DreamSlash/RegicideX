
#include "RGX_HitboxEffectApplierComponent.h"

#include "RegicideX/Actors/RGX_CharacterBase.h"

void URGX_HitboxEffectApplierComponent::SendCollisionEvents(AActor* OwnerActor, AActor* OtherActor, bool bFromSweep, const FHitResult& SweepResult)
{
	ARGX_CharacterBase* ownerCharacter = Cast<ARGX_CharacterBase>(OwnerActor);
	if (ownerCharacter)
	{
		UAbilitySystemComponent* ownerAbilitySystemComponent = ownerCharacter->GetAbilitySystemComponent();
		ARGX_CharacterBase* targetCharacter = Cast<ARGX_CharacterBase>(OtherActor);
		if (targetCharacter)
		{
			UAbilitySystemComponent* targetASC = targetCharacter ? targetCharacter->GetAbilitySystemComponent() : nullptr;

			for (const FRGX_EffectContextContainer& effect : EffectToApplyToTargetWithPayload)
			{
				FGameplayEffectContextHandle effectContext = targetASC->MakeEffectContext();
				FRGX_GameplayEffectContext* gameplayEffectContext = static_cast<FRGX_GameplayEffectContext*>(effectContext.Get());
				gameplayEffectContext->AddInstigator(ownerCharacter, ownerCharacter);
				gameplayEffectContext->OptionalObject = effect.Payload;

				const UGameplayEffect* gameplayEffect = effect.EffectToApply->GetDefaultObject<UGameplayEffect>();
				constexpr int32 level = 0;
				targetASC->ApplyGameplayEffectToSelf(gameplayEffect, level, effectContext);
			}
		}

		ActorsHit.Add(OtherActor);
	}

	if (OnHitboxOverlap.IsBound())
	{
		OnHitboxOverlap.Broadcast(OtherActor);
	}
}