// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/Executions/RGX_ExecutionPeasantJump.h"
#include "AttributeSets/MCV_AttributeSet.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"

URGX_ExecutionPeasantJump::URGX_ExecutionPeasantJump()
{

}

void URGX_ExecutionPeasantJump::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	ARGX_CharacterBase* SourceCharacter = Cast<ARGX_CharacterBase>(ExecutionParams.GetTargetAbilitySystemComponent()->GetOwner());
	ARGX_CharacterBase* TargetCharacter = Cast<ARGX_CharacterBase>(ExecutionParams.GetSourceAbilitySystemComponent()->GetOwner());

	//if (SourceCharacter && TargetCharacter)
		//SourceCharacter->OnBeingLaunched(nullptr, 1000.0f, 1500.0f);
}
