// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Notifies/RGX_ANS_ActivateHitbox.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

void URGX_ANS_ActivateHitbox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* Owner = MeshComp->GetOwner();

	URGX_HitboxesManagerComponent* HitboxesManagerComponent = Owner->FindComponentByClass<URGX_HitboxesManagerComponent>();
	if (HitboxesManagerComponent)
	{
		URGX_HitboxComponent* Hitbox = HitboxesManagerComponent->GetHitboxByTag(HitboxTag);
		Hitbox->AddEventTag(EventTag);
		Hitbox->ActivateHitbox();
	}
}

void URGX_ANS_ActivateHitbox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();

	URGX_HitboxesManagerComponent* HitboxesManagerComponent = Owner->FindComponentByClass<URGX_HitboxesManagerComponent>();
	if (HitboxesManagerComponent)
	{
		URGX_HitboxComponent* Hitbox = HitboxesManagerComponent->GetHitboxByTag(HitboxTag);
		Hitbox->RemoveEventTag(EventTag);
		Hitbox->DeactivateHitbox();
	}
}
