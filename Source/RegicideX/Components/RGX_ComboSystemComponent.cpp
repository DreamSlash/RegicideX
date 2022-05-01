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
		UE_LOG(LogTemp, Warning, TEXT("Set Next Combo Attack\n"));
		bComboFlag = true;
		NextComboInput = PlayerInput;
	}
}

void URGX_ComboSystemComponent::InitiateCombo(ERGXPlayerInputID PlayerInput)
{
	UE_LOG(LogTemp, Warning, TEXT("Initiate Combo\n"));
	NextAttack = FindNextAttack(PlayerInput);
	CurrentAttack = NextAttack; // WARNING: Next attack is not being executed yet
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
	bComboFlag = false;

	if (NextComboInput != ERGXPlayerInputID::None)
	{
		NextAttack = FindNextAttack(NextComboInput);
		NextComboInput = ERGXPlayerInputID::None;
		CurrentAttack = NextAttack; // WARNING: Next attack is not being executed yet
		UE_LOG(LogTemp, Warning, TEXT("Combo\n"));
	}
	else
	{
		CurrentAttack = FGameplayTag::RequestGameplayTag("Combo.None");
	}
}

void URGX_ComboSystemComponent::OnEnableCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("Enable Combo\n"));
	bEnableComboFlag = true;
}

void URGX_ComboSystemComponent::OnDisableCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("Disable Combo\n"));
	bEnableComboFlag = false;
}

void URGX_ComboSystemComponent::OnEndCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("On End Combo\n"));
	CurrentAttack = FGameplayTag::RequestGameplayTag("Combo.None");
	NextAttack = FGameplayTag::RequestGameplayTag("Combo.None");
	bComboFlag = false;
	bEnableComboFlag = false;
	NextComboInput = ERGXPlayerInputID::None;
}
