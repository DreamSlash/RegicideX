
#include "RGX_ANS_CombatAssistHelper.h"

#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/Components/RGX_CameraControllerComponent.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"
#include "RegicideX/Components/RGX_MovementAssistComponent.h"

#include "Kismet/KismetMathLibrary.h"

void URGX_ANS_CombatAssistHelper::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{		
	AActor* owner = MeshComp->GetOwner();
	const FVector forwardVector = owner->GetActorForwardVector();
	float speed = GetSpeed(MeshComp);

	if (URGX_CombatAssistComponent* combatAssistComp = Cast<URGX_CombatAssistComponent>(owner->GetComponentByClass(URGX_CombatAssistComponent::StaticClass())))
	{
		/*combatAssistComp->AddMovementVector(forwardVector, speed, bIsAttacking);
		combatAssistComp->EnableMovementVector();
		combatAssistComp->SetAttackMoveDuration(TotalDuration);*/
	}
}

void URGX_ANS_CombatAssistHelper::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (ARGX_PlayerCharacter* player = Cast<ARGX_PlayerCharacter>(MeshComp->GetOwner()))
	{
		bool isTargetingEnabled = false;
		if (URGX_CameraControllerComponent* cameraComp = Cast<URGX_CameraControllerComponent>(player->GetComponentByClass(URGX_CameraControllerComponent::StaticClass())))
		{
			isTargetingEnabled = cameraComp->bTargetingEnabled;
		}

		FVector inputDirection = player->GetCurrentMoveInputDirection();
		if (inputDirection.IsNearlyZero() || isTargetingEnabled)
			return;

		FVector forward = player->GetActorForwardVector();

		FVector2D current(forward);
		FVector2D target(inputDirection);

		FVector2D newForward = FMath::Vector2DInterpConstantTo(current, target, FrameDeltaTime, RotationSpeed);
		FMatrix matrix = FRotationMatrix::MakeFromX(FVector(newForward, 0.f));
		player->SetActorRotation(matrix.Rotator());
	}
}

void URGX_ANS_CombatAssistHelper::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//AActor* owner = MeshComp->GetOwner();

	//if (URGX_MovementAssistComponent* moveAssistComp = Cast<URGX_MovementAssistComponent>(owner->GetComponentByClass(URGX_MovementAssistComponent::StaticClass())))
	//{
	//	moveAssistComp->DisableMovementAssist();
	//}
	//else if (URGX_CombatAssistComponent* combatAssistComp = Cast<URGX_CombatAssistComponent>(owner->GetComponentByClass(URGX_CombatAssistComponent::StaticClass())))
	//{
	//	combatAssistComp->RemoveMovementVector();
	//}
}

float URGX_ANS_CombatAssistHelper::GetSpeed(USkeletalMeshComponent* MeshComp) const
{
	/*if (UAnimInstance* animInstance = MeshComp->GetAnimInstance())
	{
		if (ARGX_CharacterBase* character = Cast<ARGX_CharacterBase>(MeshComp->GetOwner()))
		{
			float speed = animInstance->GetCurveValue(FName("Speed"));

			if (character->IsFallingDown())
			{
				speed *= AirMultiplier;
			}

			return speed;
		}
	}*/

	return 0.0f;
}