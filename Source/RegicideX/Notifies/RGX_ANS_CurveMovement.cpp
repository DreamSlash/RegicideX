
#include "RGX_ANS_CurveMovement.h"

#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "RegicideX/Components/RGX_MovementAssistComponent.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"

void URGX_ANS_CurveMovement::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{		
	AActor* owner = MeshComp->GetOwner();
	const FVector forwardVector = owner->GetActorForwardVector();
	float speed = GetSpeed(MeshComp);

	if (URGX_MovementAssistComponent* moveAssistComp = Cast<URGX_MovementAssistComponent>(owner->GetComponentByClass(URGX_MovementAssistComponent::StaticClass())))
	{
		moveAssistComp->SetDirection(FVector(0.0, 0.0, 0.0));
		moveAssistComp->SetMagnitudeAndSpeed(Magnitude, speed);
		moveAssistComp->EnableMovementAssist();
	}
	else if (URGX_CombatAssistComponent* combatAssistComp = Cast<URGX_CombatAssistComponent>(owner->GetComponentByClass(URGX_CombatAssistComponent::StaticClass())))
	{
		combatAssistComp->AddMovementVector(forwardVector, speed, bIsAttacking);
		combatAssistComp->EnableMovementVector();
		combatAssistComp->SetAttackMoveDuration(TotalDuration);
	}
}

void URGX_ANS_CurveMovement::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (bUseTick)
	{
		AActor* owner = MeshComp->GetOwner();
		float speed = GetSpeed(MeshComp);

		if (URGX_MovementAssistComponent* moveAssistComp = Cast<URGX_MovementAssistComponent>(owner->GetComponentByClass(URGX_MovementAssistComponent::StaticClass())))
		{
			moveAssistComp->SetMagnitudeAndSpeed(Magnitude, speed);
		}
		else if (URGX_CombatAssistComponent* combatAssistComp = Cast<URGX_CombatAssistComponent>(owner->GetComponentByClass(URGX_CombatAssistComponent::StaticClass())))
		{
			combatAssistComp->SetMovementSpeed(speed);
		}
	}
}

void URGX_ANS_CurveMovement::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* owner = MeshComp->GetOwner();

	if (URGX_MovementAssistComponent* moveAssistComp = Cast<URGX_MovementAssistComponent>(owner->GetComponentByClass(URGX_MovementAssistComponent::StaticClass())))
	{
		moveAssistComp->DisableMovementAssist();
	}
	else if (URGX_CombatAssistComponent* combatAssistComp = Cast<URGX_CombatAssistComponent>(owner->GetComponentByClass(URGX_CombatAssistComponent::StaticClass())))
	{
		combatAssistComp->RemoveMovementVector();
	}
}

float URGX_ANS_CurveMovement::GetSpeed(USkeletalMeshComponent* MeshComp) const
{
	if (UAnimInstance* animInstance = MeshComp->GetAnimInstance())
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
	}

	return 0.0f;
}