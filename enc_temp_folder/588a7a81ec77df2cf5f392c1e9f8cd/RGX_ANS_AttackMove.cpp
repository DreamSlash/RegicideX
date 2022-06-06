// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Notifies/RGX_ANS_AttackMove.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"

void URGX_ANS_AttackMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AActor* Owner = MeshComp->GetOwner();
	URGX_CombatAssistComponent* CombatAssistComponent = Owner->FindComponentByClass<URGX_CombatAssistComponent>();
	if (CombatAssistComponent)
	{
		CombatAssistComponent->EnableMovementVector();
		CombatAssistComponent->SetAttackMoveDuration(TotalDuration);
	}
}

void URGX_ANS_AttackMove::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	// TODO: Implement curve movement, reading the curve in the montage and send it to combat assist component
}

void URGX_ANS_AttackMove::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	URGX_CombatAssistComponent* CombatAssistComponent = Owner->FindComponentByClass<URGX_CombatAssistComponent>();
	if (CombatAssistComponent)
	{
		CombatAssistComponent->DisableMovementVector();
	}
}
