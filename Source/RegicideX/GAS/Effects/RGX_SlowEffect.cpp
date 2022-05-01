// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_SlowEffect.h"
#include "RegicideX/GAS/AttributeSets/RGX_MovementAttributeSet.h"

URGX_SlowEffect::URGX_SlowEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FScalableFloat Duration;
	Duration.Value = SlowDuration;

	FGameplayEffectModifierMagnitude DurMagnitude(Duration);
	DurationMagnitude = DurMagnitude;
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = URGX_MovementAttributeSet::GetMovementSpeedAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Override;

	FAttributeBasedFloat AttributeBasedFloat;
	AttributeBasedFloat.Coefficient = SlowCoefficient;
	ModifierInfo.ModifierMagnitude = AttributeBasedFloat;

	Modifiers.Add(ModifierInfo);

	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Combat.Slowed")));
}