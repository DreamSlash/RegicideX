#include "RGX_Execution_Damage.h"
#include "../AttributeSets/RGX_HealthAttributeSet.h"
#include "../AttributeSets/RGX_CombatAttributeSet.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Execute Damage"));

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float DamageMitigation = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageMitigationDef, EvaluationParameters, DamageMitigation);

	float AttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);

	float FinalDamage = 0.0f;
	FinalDamage = AttackPower * FMath::Max(0.0f, (1.0f - DamageMitigation));

	FGameplayEventData EventPayload;
	ExecutionParams.GetTargetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("GameplayEvent.Combat.TakeDamage"), &EventPayload);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RGX_DamageStatics().HealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
