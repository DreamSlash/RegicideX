#include "RGX_ComboSystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

URGX_ComboSystemComponent::URGX_ComboSystemComponent()
{

}

void URGX_ComboSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URGX_ComboSystemComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void URGX_ComboSystemComponent::ManageInputToken(ERGXPlayerInputID PlayerInput)
{
	if (!IsAttacking())
	{
		InitiateCombo(PlayerInput);
	}

	SetNextComboAttack(PlayerInput);
}

FGameplayTag URGX_ComboSystemComponent::GetNextAttack()
{
	return NextAttack;
}

void URGX_ComboSystemComponent::CleanNextAttack()
{
	NextAttack = FGameplayTag::RequestGameplayTag("Combo.None");
}

bool URGX_ComboSystemComponent::IsAttacking()
{
	return CurrentAttack != FGameplayTag::RequestGameplayTag("Combo.None");
}

void URGX_ComboSystemComponent::SetNextComboAttack(ERGXPlayerInputID PlayerInput)
{
	if (bEnableComboFlag)
	{
		bComboFlag = true;
		NextComboInput = PlayerInput;
	}
}

void URGX_ComboSystemComponent::InitiateCombo(ERGXPlayerInputID PlayerInput)
{
	NextAttack = FindNextAttack(PlayerInput);
}

FGameplayTag URGX_ComboSystemComponent::FindNextAttack(ERGXPlayerInputID PlayerInput)
{
	FRGX_ComboTransition* transition = ComboMap.Find(CurrentAttack);

	for (int i = 0; i < transition->NextPotentialInputs.Num(); ++i)
	{
		if (transition->NextPotentialInputs[i] == PlayerInput)
		{
			return transition->NextPotentialAttacks[i];
		}
	}

	return FGameplayTag::RequestGameplayTag("Combo.None");
}

void URGX_ComboSystemComponent::OnCombo()
{
	NextAttack = FindNextAttack(NextComboInput);
	CurrentAttack = NextAttack; // WARNING: Next attack is not being executed yet
}

void URGX_ComboSystemComponent::OnEnableCombo()
{
	bEnableComboFlag = true;
}

void URGX_ComboSystemComponent::OnDisableCombo()
{
	bEnableComboFlag = false;
}
