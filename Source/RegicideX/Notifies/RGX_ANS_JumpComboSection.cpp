
#include "RGX_ANS_JumpComboSection.h"
#include "RegicideX\Character\RGX_PlayerCharacter.h"
#include "Animation/AnimInstance.h"

void URGX_ANS_JumpComboSection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(MeshComp->GetOwner());
	if (Player) 
	{
		Player->bCanCombo = true;
		Player->JumpComboNotifyState = this;
	}
}

void URGX_ANS_JumpComboSection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(MeshComp->GetOwner());
	if (Player)
	{
		if (Player->bCanCombo && Player->bCanJumpToComboSection && Player->bContinueCombo)
		{
			UE_LOG(LogTemp, Warning, TEXT("Jump To Next Section"));

			UAnimMontage* AnimMontage = Player->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage();
			UE_LOG(LogTemp, Warning, TEXT("CurrentActiveMontage: %s"), AnimMontage ? TEXT("TRUE") : TEXT("NULLPTR"));
			Player->GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName);
			bContinueCombo = true;
		}
	}
}

void URGX_ANS_JumpComboSection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(MeshComp->GetOwner());
	if (Player) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Notify End"));
		// If player does not continue the combo, reset the state.
		if(bContinueCombo == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Combo Interrupted"));
			Player->OnInterrupted();
		}

		// Reset flags.
		Player->bCanCombo = false;
		Player->bCanJumpToComboSection = false;
		Player->bContinueCombo = false;
		Player->JumpComboNotifyState = nullptr;
	}

	bContinueCombo = false;
}
