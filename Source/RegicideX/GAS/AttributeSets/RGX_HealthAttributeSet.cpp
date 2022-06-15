#include "RGX_HealthAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "RegicideX/GAS/GameplayEffects/RGX_GE_Death.h"
#include "RegicideX/GAS/GameplayEffects/RGX_GE_HitEffect.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"

void URGX_HealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
}

void URGX_HealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	UE_LOG(LogTemp, Warning, TEXT("New Health: %f\n"), NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
}

void URGX_HealthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();



	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		bool bIsDead = ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));
		if (bIsDead == false)
		{
			float CurrentHealth = GetHealth();
			if (CurrentHealth <= 0)
			{
				// If current health is still 0, we die
				CurrentHealth = GetHealth();
				if (CurrentHealth <= 0)
				{
					UGameplayEffect* DeathEffect = URGX_DeathEffect::StaticClass()->GetDefaultObject<UGameplayEffect>();
					ASC->ApplyGameplayEffectToSelf(DeathEffect, 1, ASC->MakeEffectContext());

					FGameplayEventData EventData;
					ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.HasDied")), &EventData);

					AActor* Owner = ASC->GetOwner();
					ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(Owner);
					if (Enemy)
					{
						float DeltaValue = 0;
						if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
						{
							// If this was additive, store the raw delta value to be passed along later
							DeltaValue = Data.EvaluatedData.Magnitude;
						}
						Enemy->HandleHealthChanged(DeltaValue);
						Enemy->HandleDeath();
					}
				}
			}
			else
			{
				// TODO: Create common class for all characters in game

				ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(Data.Target.AvatarActor);
				//ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(Data.Target.AvatarActor);

				// Compute the delta between old and new, if it is available
				float DeltaValue = 0;
				if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
				{
					// If this was additive, store the raw delta value to be passed along later
					DeltaValue = Data.EvaluatedData.Magnitude;
				}

				if (DeltaValue != 0)
				{
					if (Enemy)
					{
						Enemy->HandleHealthChanged(DeltaValue);
					}
				}

				if (DeltaValue < 0)
				{
					UE_LOG(LogTemp, Warning, TEXT("Delta Health: %f\n"), DeltaValue);
					ARGX_EnemyBase* InstigatorEnemy = Cast<ARGX_EnemyBase>(Source->AvatarActor);
					if (InstigatorEnemy)
					{
						if (Enemy)
						{
							Enemy->HandleDamage(FMath::Abs(DeltaValue), InstigatorEnemy);
						}
					}
					else
					{
						ARGX_PlayerCharacter* InstigatorPlayer = Cast<ARGX_PlayerCharacter>(Data.Target.AvatarActor);
						if (Enemy)
						{
							Enemy->HandleDamage(FMath::Abs(DeltaValue), InstigatorPlayer);
						}
					}

					UGameplayEffect* HitEffect = URGX_HitEffect::StaticClass()->GetDefaultObject<UGameplayEffect>();
					ASC->ApplyGameplayEffectToSelf(HitEffect, 1, ASC->MakeEffectContext());

					{
						FGameplayEventData EventData;
						EventData.Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();
						ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Combat.TakeDamage")), &EventData);
					}


					if (CurrentHealth < GetMaxHealth() * 0.25f)
					{
						FGameplayEventData EventData;
						ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Enemy.Weakened")), &EventData);
					}
				}
			}
		}
	}
}