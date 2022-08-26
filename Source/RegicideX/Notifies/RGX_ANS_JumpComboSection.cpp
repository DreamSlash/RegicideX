
#include "RGX_ANS_JumpComboSection.h"
#include "RegicideX\Character\RGX_PlayerCharacter.h"

void URGX_ANS_JumpComboSection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(MeshComp->GetOwner());
	if (Player) {
		Player->bCanCombo = true;
		Player->JumpComboNotifyState = this;
	}
}

void URGX_ANS_JumpComboSection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
}

void URGX_ANS_JumpComboSection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ARGX_PlayerCharacter* Player = Cast<ARGX_PlayerCharacter>(MeshComp->GetOwner());
	if (Player) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("Jump Combo Section End. ComboFlag: %s"), Player->bComboFlag ? TEXT("TRUE") : TEXT("FALSE"));
		if (Player->bComboFlag == false)
		{
			Player->OnInterrupted();
		}

		Player->bComboFlag = false;
		Player->bCanCombo = false;
		Player->JumpComboNotifyState = nullptr;
	}
}
