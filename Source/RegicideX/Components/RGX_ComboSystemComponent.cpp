#include "RGX_ComboSystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"

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

FGameplayTag URGX_ComboSystemComponent::ManageInputToken(ERGX_ComboTokenID PlayerInput, bool bIsOnAir, bool bCanAirCombo)
{
	if (!IsAttacking())
	{
		if (bIsOnAir == true && bCanAirCombo == false)
		{
			return FGameplayTag::RequestGameplayTag(FName("Combo.None"));
		}

		InitiateCombo(PlayerInput, bIsOnAir);
		return CurrentAttack;
	}

	SetNextComboAttack(PlayerInput);

	return FGameplayTag::RequestGameplayTag(FName("Combo.None"));
}

FGameplayTag URGX_ComboSystemComponent::GetNextAttack()
{
	return NextAttack;
}

void URGX_ComboSystemComponent::CleanStatus(int32 ActivatedAbilities)
{
	NextAttack = FGameplayTag::RequestGameplayTag("Combo.None");

	if (ActivatedAbilities == 0)
	{
		OnEndCombo();
	}
}

bool URGX_ComboSystemComponent::IsAttacking()
{
	return CurrentAttack != FGameplayTag::RequestGameplayTag("Combo.None");
}

void URGX_ComboSystemComponent::SetNextComboAttack(ERGX_ComboTokenID PlayerInput)
{
	if (bEnableComboFlag)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Set Next Combo Attack\n"));
		bComboFlag = true;
		NextComboInput = PlayerInput;
	}
}

void URGX_ComboSystemComponent::InitiateCombo(ERGX_ComboTokenID PlayerInput, bool bIsOnAir)
{
	//UE_LOG(LogTemp, Warning, TEXT("Initiate Combo\n"));
	// TODO: hardcodeada padre
	if (bIsOnAir)
	{
		FRGX_ComboTransition* transition = ComboMap.Find(FGameplayTag::RequestGameplayTag(FName("Combo.Air.Light")));
		if (transition)
		{
			NextAttack = FGameplayTag::RequestGameplayTag(FName("Combo.Air.Light"));
			CurrentAttack = NextAttack;
		}
	}
	else
	{
		NextAttack = FindNextAttack(PlayerInput);
		CurrentAttack = NextAttack; // WARNING: Next attack is not being executed yet
	}
}

FGameplayTag URGX_ComboSystemComponent::FindNextAttack(ERGX_ComboTokenID PlayerInput)
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
	//UE_LOG(LogTemp, Warning, TEXT("OnCombo\n"));

	bComboFlag = false;
	bEnableComboFlag = false;

	if (NextComboInput != ERGX_ComboTokenID::None)
	{
		NextAttack = FindNextAttack(NextComboInput);
		NextComboInput = ERGX_ComboTokenID::None;
		CurrentAttack = NextAttack; // WARNING: Next attack is not being executed yet
		//UE_LOG(LogTemp, Warning, TEXT("Combo\n"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("No Combo\n"));
		CurrentAttack = FGameplayTag::RequestGameplayTag("Combo.None");
		OnEndCombo();
	}
}

void URGX_ComboSystemComponent::OnEnableCombo()
{
	//UE_LOG(LogTemp, Warning, TEXT("Enable Combo\n"));
	bEnableComboFlag = true;
}

void URGX_ComboSystemComponent::OnDisableCombo()
{
	//UE_LOG(LogTemp, Warning, TEXT("Disable Combo\n"));
	bEnableComboFlag = false;
}

void URGX_ComboSystemComponent::OnEndCombo()
{
	//UE_LOG(LogTemp, Warning, TEXT("On End Combo\n"));
	CurrentAttack = FGameplayTag::RequestGameplayTag("Combo.None");
	NextAttack = FGameplayTag::RequestGameplayTag("Combo.None");
	bComboFlag = false;
	bEnableComboFlag = false;
	NextComboInput = ERGX_ComboTokenID::None;

	ARGX_PlayerCharacter* Owner = Cast<ARGX_PlayerCharacter>(GetOwner());
	if (Owner)
	{
		UCharacterMovementComponent* CharacterMovementComponent = Owner->FindComponentByClass<UCharacterMovementComponent>();
		CharacterMovementComponent->GravityScale = Owner->GetCurrentGravityScale();
	}
}

void URGX_ComboSystemComponent::DrawDebugInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("Current Attack: %s\n"), *CurrentAttack.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Next Attack: %s\n"), *NextAttack.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Next Combo Input: %d\n"), (int32)NextComboInput);

	if (bEnableComboFlag)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enable Combo Flag: TRUE\n"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enable Combo Flag: FALSE\n"));
	}

	if (bComboFlag)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Flag: TRUE\n"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Flag: FALSE\n"));
	}
}

FGameplayTag URGX_ComboSystemComponent::GetCurrentAttackTag() const
{
	return CurrentAttack;
}
