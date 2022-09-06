#include "RGX_Execution_Damage.h"
#include "RegicideX/GAS/AttributeSets/RGX_AttributeSet.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"

struct RGX_DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)

	RGX_DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_AttributeSet, Damage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_AttributeSet, AttackPower, Source, true);
	}
};

static const RGX_DamageStatics& DamageStatics()
{
	static RGX_DamageStatics damageStatics;
	return damageStatics;
}

UExecution_Damage::UExecution_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
}

void UExecution_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// If invulnerable or dead, not damage execution to apply.
	if (TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Invulnerable"))) == true ||
		TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead"))) == true)
	{
		return;
	}

	// Get payload
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	FRGX_GameplayEffectContext* FRGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());

	const URGX_DamageEventDataAsset* DamageEventData = Cast<URGX_DamageEventDataAsset>(FRGXContext->OptionalObject);

	if (DamageEventData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong payload type"));
		return;
	}

	FString ContextString;
	const FRealCurve* DamageCurve = DamageEventData->DamageLevelCurve->FindCurve(DamageEventData->DamageCurveName, ContextString);
	const FRealCurve* ScalingCurve = DamageEventData->DamageLevelCurve->FindCurve(DamageEventData->AttributeScalingCurveName, ContextString);
	const float DamageAmount = DamageCurve->Eval(FRGXContext->GetAbilityLevel());
	const float ScalingAttributeFactor = ScalingCurve->Eval(FRGXContext->GetAbilityLevel());

	// Get attribute values
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.SourceTags = TargetTags;

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);

	float AttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);

	float FinalDamage = 0.0f;
	FinalDamage = DamageAmount + AttackPower * ScalingAttributeFactor;

	if (FinalDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RGX_DamageStatics().DamageProperty, EGameplayModOp::Additive, FinalDamage));
	}
}