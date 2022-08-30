
#include "RGX_ANS_StopFromFalling.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"

void URGX_ANS_StopFromFalling::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{		
	ARGX_CharacterBase* OwnerCharacter = Cast<ARGX_CharacterBase>(MeshComp->GetOwner());
	AAIController* OwnerController = OwnerCharacter ? Cast<AAIController>(OwnerCharacter->GetController()) : nullptr;
	if (OwnerCharacter)
	{
		if (OwnerController)
		{
			OwnerController->GetBrainComponent()->StopLogic(FString("On should not be doing any logic."));
			//OwnerController->SetFocus(nullptr);
		}

		if (OwnerCharacter->GetMovementComponent()->IsFalling())
		{
			OwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;
			ACharacter* Character = Cast<ACharacter>(OwnerCharacter);
			Character->LaunchCharacter(FVector(0.0f, 0.0f, -1.0f), true, true);
		}
	}
}

void URGX_ANS_StopFromFalling::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
}

void URGX_ANS_StopFromFalling::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character)
	{
		AAIController* OwnerController = Cast<AAIController>(Character->GetController());
		if (OwnerController)
		{
			OwnerController->GetBrainComponent()->StartLogic();
		}

		Character->GetCharacterMovement()->GravityScale = 3.0f;
	}
}
