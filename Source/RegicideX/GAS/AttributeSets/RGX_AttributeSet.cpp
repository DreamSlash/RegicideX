
#include "RGX_AttributeSet.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

URGX_AttributeSet::URGX_AttributeSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
	, AttackPower(1.0f)
{
}

void URGX_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// If the MaxHealth is changed we want to adjust Healt accordingly.
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void URGX_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle Context	= Data.EffectSpec.GetContext();
	const UAbilitySystemComponent* Source		= Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags		= *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Compute delta between new and old if available.
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	AActor*				TargetActor = nullptr;
	AController*		TargetController = nullptr;
	ARGX_CharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor			= Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController	= Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter		= Cast<ARGX_CharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute() && TargetCharacter)
	{
		// Get the source actor
		AActor*	SourceActor					= nullptr;
		AController* SourceController		= nullptr;
		ARGX_CharacterBase* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			// Use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<ARGX_CharacterBase>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<ARGX_CharacterBase>(SourceActor);
			}

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		const float LocalDamage = TargetCharacter->HandleDamageMitigation(GetDamage(), HitResult, SourceTags, SourceCharacter, SourceActor);
		SetDamage(0.0f);

		if (LocalDamage > 0.0f)
		{
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamage, 0.0f, GetMaxHealth()));

			TargetCharacter->HandleDamage(LocalDamage, HitResult, SourceTags, SourceCharacter, SourceActor);
			TargetCharacter->HandleHealthChanged(-LocalDamage, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other Health canges such as healing
		// Clamp first
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
	}
}

void URGX_AttributeSet::AdjustAttributeForMaxChange(
	FGameplayAttributeData& AffectedAttribute, 
	const FGameplayAttributeData& MaxAttribute, 
	float NewMaxValue, 
	const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}
