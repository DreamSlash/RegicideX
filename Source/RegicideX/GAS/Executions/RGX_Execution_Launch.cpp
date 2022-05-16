#include "RGX_Execution_Launch.h"
#include "../AttributeSets/RGX_HealthAttributeSet.h"
#include "../AttributeSets/RGX_CombatAttributeSet.h"
#include "../RGX_PayloadObjects.h"

UExecution_Launch::UExecution_Launch()
{
}

void UExecution_Launch::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.SourceTags = TargetTags;
	
	FGameplayEventData EventData;
	URGX_LaunchEventPayload* LaunchPayloadObject = NewObject<URGX_LaunchEventPayload>();
	LaunchPayloadObject->LaunchHorizontalForce = 0.0f; //TODO: Do not hardcode this
	LaunchPayloadObject->LaunchVerticalForce = 1300.0f;
	LaunchPayloadObject->bOverrideHorizontal = true;
	LaunchPayloadObject->bOverrideVertical = true;

	EventData.OptionalObject = LaunchPayloadObject;
	UE_LOG(LogTemp, Warning, TEXT("Execution Launch\n"));
	ExecutionParams.GetTargetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("GameplayEvent.Launched"), &EventData);

	//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RGX_DamageStatics().HealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
