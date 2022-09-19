#include "RGX_GA_EvasionAbility.h"
#include "GameFramework/Character.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void URGX_EvasionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Character == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}

	// Rotate character towards input
	FVector2D MovementInput = Character->LastInputDirection;
	//UE_LOG(LogTemp, Warning, TEXT("Input Direction: %f, %f"), MovementInput.X, MovementInput.Y);

	const FRotator Rotation = Character->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	FVector Direction = ForwardDirection * MovementInput.X + RightDirection * MovementInput.Y;
	Direction.Normalize();

	Character->SetActorRotation(Direction.Rotation());

	bool bIsBeingAttacked = Character->IsBeingAttacked();

	if (bIsBeingAttacked)
	{
		// Perform dodge ability
		FGameplayEventData EventData;
		ActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Action.Dodge")), &EventData);
	}
	else
	{
		// Perform dash ability
		FGameplayEventData EventData;
		ActorInfo->AbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Action.Dash")), &EventData);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
