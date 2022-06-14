#include "RGX_GA_LaunchedAbility.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/CharacterMovementComponent.h"

bool URGX_LaunchedAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (Character == nullptr)
		return false;

	return true;
}

void URGX_LaunchedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{	
	const URGX_LaunchEventDataAsset* LaunchPayload = Cast<URGX_LaunchEventDataAsset>(TriggerEventData->OptionalObject);

	//UE_LOG(LogTemp, Warning, TEXT("Launch Ability\n"));

	if (LaunchPayload == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}

	FVector ActorLocation = ActorInfo->AvatarActor->GetActorLocation();
	FVector ForceOrigin = TriggerEventData->Instigator->GetActorLocation();
	float HorizontalForce = LaunchPayload->LaunchHorizontalForce;
	float VerticalForce = LaunchPayload->LaunchVerticalForce;
	//UE_LOG(LogTemp, Warning, TEXT("Vertical Force: %f\n"), VerticalForce);
	FVector LaunchHorizontalDirection = ActorLocation - ForceOrigin;
	LaunchHorizontalDirection.Z = 0.0f;
	LaunchHorizontalDirection.Normalize();

	FVector LaunchForce = LaunchHorizontalDirection * HorizontalForce + FVector(0.0f, 0.0f, 1.0f) * VerticalForce;

	//UE_LOG(LogTemp, Warning, TEXT("Launch force: %f, %f, %f\n"), LaunchForce.X, LaunchForce.Y, LaunchForce.Z);

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor);
	//bool bIsOnAir = Character->GetCharacterMovement()->IsFalling();
	bool bIsOnAir = Character->GetCharacterMovement()->GravityScale == 0;

	if (bIsOnAir == true)
	{
		UAbilityTask_WaitDelay* TaskWaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, LaunchDuration);
		TaskWaitDelay->OnFinish.AddDynamic(this, &URGX_LaunchedAbility::OnFinishDelay);
		TaskWaitDelay->ReadyForActivation();
		Character->LaunchCharacter(LaunchForce, true, true);
	}
	else
	{
		Character->LaunchCharacter(LaunchForce, true, true);
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}

void URGX_LaunchedAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_LaunchedAbility::OnFinishDelay()
{
	ACharacter* Character = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	Character->LaunchCharacter(FVector(0.1f, 0.1f, 0.0f), true, true);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
