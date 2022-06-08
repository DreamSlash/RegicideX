// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/Executions/RGX_Execution_LevelUp.h"
#include "RegicideX/GAS/AttributeSets/RGX_HealthAttributeSet.h"
#include "RegicideX/GAS/AttributeSets/RGX_CombatAttributeSet.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

struct RGX_LevelUpStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth)
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)

	RGX_LevelUpStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_HealthAttributeSet, MaxHealth, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_CombatAttributeSet, AttackPower, Target, true);
	}
};

static const RGX_LevelUpStatics& LevelUpStatics()
{
	static RGX_LevelUpStatics damageStatics;
	return damageStatics;
}

URGX_Execution_LevelUp::URGX_Execution_LevelUp()
{
	RelevantAttributesToCapture.Add(RGX_LevelUpStatics().MaxHealthDef);
	RelevantAttributesToCapture.Add(RGX_LevelUpStatics().AttackPowerDef);
}

void URGX_Execution_LevelUp::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/*
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	FRGX_GameplayEffectContext* FRGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RGX_DamageStatics().MaxHealthProperty, EGameplayModOp::Override, FRGXContext->NewMaxHealth));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(RGX_DamageStatics().AttackPowerProperty, EGameplayModOp::Override, FRGXContext->NewAttackPower));
*/
}
