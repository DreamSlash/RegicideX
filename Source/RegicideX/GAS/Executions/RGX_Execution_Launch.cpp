#include "RGX_Execution_Launch.h"
#include "AttributeSets/MCV_AttributeSet.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"

UExecution_Launch::UExecution_Launch()
{
}

void UExecution_Launch::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	ARGX_CharacterBase* TargetCharacter = Cast<ARGX_CharacterBase>(ExecutionParams.GetTargetAbilitySystemComponent()->GetOwner());
	ARGX_CharacterBase* InstigatorCharacter = Cast<ARGX_CharacterBase>(ExecutionParams.GetSourceAbilitySystemComponent()->GetOwner());

	if(TargetCharacter && InstigatorCharacter)
		TargetCharacter->OnBeingLaunched(InstigatorCharacter, 0.0f, 1500.0f);
	//const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	//const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	//const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//FAggregatorEvaluateParameters EvaluationParameters;
	//EvaluationParameters.SourceTags = SourceTags;
	//EvaluationParameters.SourceTags = TargetTags;
	//
	//FGameplayEventData EventData;
	//URGX_LaunchEventPayload* LaunchPayloadObject = NewObject<URGX_LaunchEventPayload>();
	//LaunchPayloadObject->LaunchHorizontalForce = 0.0f; //TODO: Do not hardcode this
	//LaunchPayloadObject->LaunchVerticalForce = 1300.0f;
	//LaunchPayloadObject->bOverrideHorizontal = true;
	//LaunchPayloadObject->bOverrideVertical = true;

	//EventData.OptionalObject = LaunchPayloadObject;
	//UE_LOG(LogTemp, Warning, TEXT("Execution Launch\n"));
	//ExecutionParams.GetTargetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("GameplayEvent.Launched"), &EventData);

	//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RGX_DamageStatics().HealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
