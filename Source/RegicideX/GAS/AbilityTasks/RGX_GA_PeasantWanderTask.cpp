// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_PeasantWanderTask.h"
#include "AIController.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

void URGX_GA_PeasantWanderTask::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_Peasant* Self = Cast<ARGX_Peasant>(ActorInfo->AvatarActor);

	FVector2D Offset = FVector2D(FMath::RandRange(-100.0f, 100.0f), FMath::RandRange(-100.0f, 100.0f));
	//FVector WanderingPoint = FVector(Self->IdlePosition.X + Offset.X, Self->IdlePosition.Y + Offset.Y, Self->IdlePosition.Z);

}

void URGX_GA_PeasantWanderTask::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
