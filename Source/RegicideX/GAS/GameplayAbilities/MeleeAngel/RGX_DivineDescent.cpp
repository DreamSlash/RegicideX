
#include "RGX_DivineDescent.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"

bool URGX_DivineDescent::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if(bResult == false)
		return false;

	return true;
}

void URGX_DivineDescent::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_DivineDescent::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	bTargetSelected = false;
}

void URGX_DivineDescent::Tick(float DeltaTime)
{
	if (!CurrentActorInfo)
		return;

	ARGX_MeleeAngel* owner = Cast<ARGX_MeleeAngel>(GetAvatarActorFromActorInfo());
	if (owner == nullptr || owner->bCharging == false) {
		return;
	}

	if (bFinished)
		return;

	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(owner->GetMovementComponent());
	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->SetMovementMode(MOVE_Custom, 0);
	}

	FVector NewLocation = FVector::ZeroVector;

	TargetLocation = owner->DivineDescentTargetLocation;
	TargetLocation.Z = 25.0f;
	StartLocation = owner->GetActorLocation();

	FVector Direction = TargetLocation - StartLocation;
	float Distance = Direction.Size();
	Direction.Normalize();
	NewLocation = StartLocation + Direction * Speed * DeltaTime;

	owner->SetActorLocation(NewLocation);

	if (Distance < ThresholdDistance)
	{
		MontageJumpToSection(FName("Fall Section"));
		UE_LOG(LogTemp, Warning, TEXT("CHAAAAAAAAAAAAARGE!"));
		owner->bCharging = false;
	}
}

TStatId URGX_DivineDescent::GetStatId() const
{
	return TStatId();
}
