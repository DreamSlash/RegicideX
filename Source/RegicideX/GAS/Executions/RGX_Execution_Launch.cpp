#include "RGX_Execution_Launch.h"
#include "AttributeSets/MCV_AttributeSet.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

UExecution_Launch::UExecution_Launch()
{
}

void UExecution_Launch::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	ARGX_CharacterBase* TargetCharacter = Cast<ARGX_CharacterBase>(ExecutionParams.GetTargetAbilitySystemComponent()->GetOwner());
	ARGX_CharacterBase* InstigatorCharacter = Cast<ARGX_CharacterBase>(ExecutionParams.GetSourceAbilitySystemComponent()->GetOwner());

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Get payload
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	FRGX_GameplayEffectContext* FRGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());

	URGX_LaunchEventDataAsset* LaunchEventData = Cast<URGX_LaunchEventDataAsset>(FRGXContext->OptionalObject);

	if (LaunchEventData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong payload type"));
		return;
	}

	if (TargetCharacter && InstigatorCharacter)
	{
		TargetCharacter->OnBeingLaunched(InstigatorCharacter, LaunchEventData);
	}
}
