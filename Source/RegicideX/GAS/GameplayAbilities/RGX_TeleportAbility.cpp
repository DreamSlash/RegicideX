// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_TeleportAbility.h"

URGX_TeleportAbility::URGX_TeleportAbility()
{

}

void URGX_TeleportAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{

	AActor* Actor = Cast<AActor>(ActorInfo->AvatarActor);
	Actor->SetActorLocation(LocationToTeleport);

}
