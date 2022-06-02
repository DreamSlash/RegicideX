// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_GA_PeasantReactionHit.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

void URGX_GA_PeasantReactionHit::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->OwnerActor);
	AAIController* Controller = Cast<AAIController>(Character->GetController());

	// If reacting to hit, should not move
	if (Controller) {
		Controller->GetBrainComponent()->StopLogic(FString("Animation playing"));
		Controller->SetFocus(nullptr);
	}

	// Launch Peasant a small bit backwards
	ARGX_Peasant* Peasant		= Cast<ARGX_Peasant>(Character);
	FVector PeasantLocation		= Peasant->GetActorLocation();
	FVector TargetActorForward	= Peasant->TargetActor->GetActorForwardVector();

	UCharacterMovementComponent* PeasantMovementComponent = Peasant->GetCharacterMovement();

	if (PeasantMovementComponent->IsFalling())
	{
		PeasantMovementComponent->GravityScale = 0.0f;
		Character->LaunchCharacter(FVector(0.0, 0.0, -1.0), true, true); // ¿ Don't know why it is needed to make enemy stay on air ?
	}
	else {
		Character->LaunchCharacter(TargetActorForward * 1000.0f, true, false);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_GA_PeasantReactionHit::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ACharacter* Character = Cast<ACharacter>(ActorInfo->OwnerActor);
	if (Character)
	{
		// Restart brain logic
		AAIController* Controller = Cast<AAIController>(Character->GetController());
		ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(Character);
		if (Controller) {
			Controller->GetBrainComponent()->StartLogic();
			if(Peasant->TargetActor)
				Controller->SetFocus(Peasant->TargetActor);
		}

		// Get gravity scale back
		UCharacterMovementComponent* PeasantMovementComponent = Peasant->GetCharacterMovement();
		PeasantMovementComponent->GravityScale = 3.0f;
	}
}

void URGX_GA_PeasantReactionHit::OnEndMontage()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
