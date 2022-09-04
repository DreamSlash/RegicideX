
#include "RGX_ANS_StopFromFalling.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"

void URGX_ANS_StopFromFalling::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{		
	ARGX_EnemyBase* OwnerCharacter = Cast<ARGX_EnemyBase>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		ARGX_PlayerCharacter* PlayerCharacter = nullptr;
		if(OwnerCharacter->TargetActor != nullptr)
			 PlayerCharacter = Cast<ARGX_PlayerCharacter>(OwnerCharacter->TargetActor);

		if (OwnerCharacter)
		{
			AAIController* OwnerController = Cast<AAIController>(OwnerCharacter->GetController());
			if (OwnerController)
			{
				OwnerController->GetBrainComponent()->StopLogic(FString("On should not be doing any logic."));
			}

			if (OwnerCharacter->GetMovementComponent()->IsFalling())
			{
				OwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;
				ACharacter* Character = Cast<ACharacter>(OwnerCharacter);
				Character->LaunchCharacter(FVector(0.0f, 0.0f, -1.0f), true, true);
			}
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
