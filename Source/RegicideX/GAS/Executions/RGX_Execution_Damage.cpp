#include "RGX_Execution_Damage.h"
#include "../AttributeSets/RGX_HealthAttributeSet.h"
#include "../AttributeSets/RGX_CombatAttributeSet.h"

#pragma optimize( "", off )
struct RGX_DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageMitigation)
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)

	RGX_DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_HealthAttributeSet, Health, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_CombatAttributeSet, DamageMitigation, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_CombatAttributeSet, AttackPower, Source, true);
	}
};

static const RGX_DamageStatics& DamageStatics()
{
	static RGX_DamageStatics damageStatics;
	return damageStatics;
}

UExecution_Damage::UExecution_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageMitigationDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
}

void UExecution_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	bool dmg = TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Combat.TakeDamage"))) == true;

	if (TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Invulnerable"))) == true)
	{
		return;
	}

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.SourceTags = TargetTags;

	float DamageMitigation = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageMitigationDef, EvaluationParameters, DamageMitigation);

	float AttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);

	//UE_LOG(LogTemp, Warning, TEXT("Attack Damage: %f\n"), AttackPower);

	float FinalDamage = 0.0f;
	FinalDamage = AttackPower * FMath::Max(0.0f, (1.0f - DamageMitigation));

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RGX_DamageStatics().HealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
#pragma optimize( "", on )