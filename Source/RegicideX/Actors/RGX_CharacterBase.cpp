
#include "RGX_CharacterBase.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

ARGX_CharacterBase::ARGX_CharacterBase()
{
	AbilitySystemComponent	= CreateDefaultSubobject<UMCV_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet			= CreateDefaultSubobject<URGX_AttributeSet>(TEXT("AttributeSet"));

	CharacterLevel = 1;
	bAbilitiesInitialized = false;
}

void ARGX_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init abilities
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}
}

void ARGX_CharacterBase::UnPossessed()
{
}

UAbilitySystemComponent* ARGX_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ARGX_CharacterBase::GetHealth() const
{
	if (!AttributeSet)
		return 1.0f;
	return AttributeSet->GetHealth();;
}

float ARGX_CharacterBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

int32 ARGX_CharacterBase::GetCharacterLevel() const
{
	return CharacterLevel;
}

bool ARGX_CharacterBase::SetCharacterLevel(int32 NewLevel)
{
	if (CharacterLevel != NewLevel && NewLevel > 0)
	{
		// Our level changed so we need to refresh abilities
		RemoveStartupGameplayAbilities();
		CharacterLevel = NewLevel;
		AddStartupGameplayAbilities();

		return true;
	}
	return false;
}

bool ARGX_CharacterBase::IsAlive()
{
	return GetHealth() > 0.0f ? true : false;
}

void ARGX_CharacterBase::HandleDamage(
	float DamageAmount, 
	const FHitResult& HitInfo, 
	const FGameplayTagContainer& DamageTags, 
	ARGX_CharacterBase* InstigatorCharacter, 
	AActor* DamageCauser)
{
	// Add the Status.Dead tag if not alive to avoid getting affected by Gameplay Effects.
	if(IsAlive() == false)
		GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));

	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ARGX_CharacterBase::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	OnHealthChanged(DeltaValue, EventTags);
}

void ARGX_CharacterBase::HandleDeath()
{
	OnHandleDeath();
}

void ARGX_CharacterBase::AddStartupGameplayAbilities()
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No ability system component when adding startup abilities."));
		return;
	}

	// Get granted abilities.
	for (const FRGX_AbilityInitData& StartupAbility : GameplayAbilities)
	{
		UClass* AbilityClass = StartupAbility.Ability.Get();
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, static_cast<int32>(StartupAbility.InputID));
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}

	// Apply effects, such as initializing the base attributes.
	for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActivateEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	bAbilitiesInitialized = true;
}

void ARGX_CharacterBase::RemoveStartupGameplayAbilities()
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No ability system component when removing startup abilities."));
		return;
	}

	// Remove any abilities added from a previous call
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		FRGX_AbilityInitData Ability;
		Ability.InputID = ERGX_AbilityInputID(Spec.InputID);
		if (Spec.SourceObject == this)
		{
			for (const auto& GA : GameplayAbilities)
			{
				if (GA.Ability == Spec.Ability->GetClass()) {
					AbilitiesToRemove.Add(Spec.Handle);
				}
			}
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	// Remove all of the passive gameplay effects that were applied by this character
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	AbilitySystemComponent->RemoveActiveEffects(Query);

	bAbilitiesInitialized = false;
}

FGenericTeamId ARGX_CharacterBase::GetGenericTeamId() const
{
	static const FGenericTeamId PlayerTeam(0);
	static const FGenericTeamId AITeam(1);
	return Cast<APlayerController>(GetController()) ? PlayerTeam : AITeam;
}
