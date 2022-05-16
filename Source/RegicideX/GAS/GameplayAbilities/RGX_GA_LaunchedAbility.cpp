#include "RGX_GA_LaunchedAbility.h"
#include "../RGX_PayloadObjects.h"
#include "GameFramework/Character.h"

void URGX_LaunchedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const URGX_LaunchEventPayload* LaunchPayload = Cast<URGX_LaunchEventPayload>(TriggerEventData->OptionalObject);

	UE_LOG(LogTemp, Warning, TEXT("Launch Ability\n"));

	if (LaunchPayload == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}

	FVector ActorLocation = ActorInfo->AvatarActor->GetActorLocation();
	FVector ForceOrigin = LaunchPayload->ForceOrigin;
	float HorizontalForce = LaunchPayload->LaunchHorizontalForce;
	float VerticalForce = LaunchPayload->LaunchVerticalForce;
	UE_LOG(LogTemp, Warning, TEXT("Vertical Force: %f\n"), VerticalForce);
	FVector LaunchHorizontalDirection = ActorLocation - ForceOrigin;
	LaunchHorizontalDirection.Z = 0.0f;
	LaunchHorizontalDirection.Normalize();

	FVector LaunchForce = LaunchHorizontalDirection * HorizontalForce + FVector(0.0f, 0.0f, 1.0f) * VerticalForce;

	UE_LOG(LogTemp, Warning, TEXT("Launch force: %f, %f, %f\n"), LaunchForce.X, LaunchForce.Y, LaunchForce.Z);

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (Character)
	{
		Character->LaunchCharacter(LaunchForce, true, true);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
