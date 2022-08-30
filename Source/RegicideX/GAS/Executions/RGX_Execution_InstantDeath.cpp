
#include "RGX_Execution_InstantDeath.h"
#include "RegicideX/GAS/AttributeSets/RGX_AttributeSet.h"

struct RGX_InstantDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)

	RGX_InstantDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_AttributeSet, Health, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_AttributeSet, Damage, Source, true);
	}
};

static const RGX_InstantDamageStatics& InstantDeathStatics()
{
	static RGX_InstantDamageStatics damageStatics;
	return damageStatics;
}

UExecution_InstantDeath::UExecution_InstantDeath()
{
	RelevantAttributesToCapture.Add(InstantDeathStatics().HealthDef);
	RelevantAttributesToCapture.Add(InstantDeathStatics().DamageDef);
}

void UExecution_InstantDeath::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* AbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	// If target is invulnerable, it cannot take damage.
	if (TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Invulnerable"))) == true)
	{
		return;
	}

	FAggregatorEvaluateParameters EvaluationParameters;

	float Health = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InstantDeathStatics().HealthDef, EvaluationParameters, Health);

	float FinalHealth = (Health + 1);

	if (FinalHealth >= 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RGX_InstantDamageStatics().DamageProperty, EGameplayModOp::Additive, FinalHealth));
	}
}
