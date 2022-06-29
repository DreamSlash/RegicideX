
#include "RegicideX/GAS/Executions/RGX_ExecutionEnemyExecution.h"
#include "RegicideX/GAS/AttributeSets/RGX_AttributeSet.h"

//#pragma optimize( "", off )
struct RGX_HealthStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth)

	RGX_HealthStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_AttributeSet, Health, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_AttributeSet, MaxHealth, Target, true);
	}
};

static const RGX_HealthStatics& HealthStatics()
{
	static RGX_HealthStatics healthStatics;
	return healthStatics;
}

URGX_ExecutionEnemyExecution::URGX_ExecutionEnemyExecution()
{
	RelevantAttributesToCapture.Add(HealthStatics().HealthDef);
	RelevantAttributesToCapture.Add(HealthStatics().MaxHealthDef);
}
void URGX_ExecutionEnemyExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Health = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthStatics().HealthDef, EvaluationParameters, Health);

	float MaxHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthStatics().MaxHealthDef, EvaluationParameters, MaxHealth);

	//UE_LOG(LogTemp, Warning, TEXT("Attack Damage: %f\n"), AttackPower);

	float HealthHealing = 0.0f;
	HealthHealing = (1 - (1 - (MaxHealth - Health) / MaxHealth)) * 0.3f * MaxHealth;

	UE_LOG(LogTemp, Warning, TEXT("Health Healing: %f\n"), HealthHealing);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RGX_HealthStatics().HealthProperty, EGameplayModOp::Additive, HealthHealing));
}
