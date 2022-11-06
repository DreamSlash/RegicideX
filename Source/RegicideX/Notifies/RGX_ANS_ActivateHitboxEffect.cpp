// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Notifies/RGX_ANS_ActivateHitboxEffect.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

void URGX_ANS_ActivateHitboxEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (URGX_HitboxesManagerComponent* HitboxesManagerComponent = Owner->FindComponentByClass<URGX_HitboxesManagerComponent>())
		{
			if (URGX_HitboxComponent* Hitbox = HitboxesManagerComponent->GetHitboxByTag(HitboxTag))
			{
				Hitbox->ActivateHitbox(true);
			}
		}
	}
}

void URGX_ANS_ActivateHitboxEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (URGX_HitboxesManagerComponent* HitboxesManagerComponent = Owner->FindComponentByClass<URGX_HitboxesManagerComponent>())
		{
			if (URGX_HitboxComponent* Hitbox = HitboxesManagerComponent->GetHitboxByTag(HitboxTag))
			{
				Hitbox->DeactivateHitbox();
			}
		}
	}
}
