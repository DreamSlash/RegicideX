// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/MeleeAngel/RGX_MEAChargeAbility.h"
#include "GameFramework/Character.h"
#include "RegicideX/GAS/AbilityTasks/RGX_MoveToLocationCharge.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

bool URGX_MEAChargeAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(GetAvatarActorFromActorInfo());
	if (Enemy == nullptr)
		return false;

	URGX_HitboxesManagerComponent* HitboxManagerComponent = Enemy->FindComponentByClass<URGX_HitboxesManagerComponent>();
	if (HitboxManagerComponent == nullptr)
		return false;

	return true;
}

void URGX_MEAChargeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Pass info to hitbox
	URGX_HitboxesManagerComponent* HitboxManagerComponent = GetAvatarActorFromActorInfo()->FindComponentByClass<URGX_HitboxesManagerComponent>();
	URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);
	if (Hitbox)
	{
		// TODO: Use this line to create a context with the ability spec so the effects can access its information, like ability level or a struct with custom information
		FGameplayEffectContextHandle ContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), ActorInfo);
		FRGX_GameplayEffectContext* Context = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());

		float AbilityLevel = GetAbilityLevel();
		FString ContextString;
		FRealCurve* DamageCurve = DamageLevelCurve->FindCurve(DamageCurveName, ContextString);
		FRealCurve* ScalingCurve = DamageLevelCurve->FindCurve(AttributeScalingCurveName, ContextString);
		Context->DamageAmount = DamageCurve->Eval(AbilityLevel);
		Context->ScalingAttributeFactor = ScalingCurve->Eval(AbilityLevel);

		FRGX_AbilityEffectsInfo AbilityEffectsInfo;
		AbilityEffectsInfo.EffectContextHandle = ContextHandle;
		AbilityEffectsInfo.GameplayEffectsToTarget = EffectsToApplyToTarget;
		AbilityEffectsInfo.GameplayEffectsToOwner = EffectsToApplyToOwner;
		AbilityEffectsInfo.GameplayEventsToTarget = EventsToApplyToTarget;
		AbilityEffectsInfo.GameplayEventsToOwner = EventsToApplyToOwner;
		Hitbox->SetAbilityEffectsInfo(AbilityEffectsInfo);
		Hitbox->ActivateEffect();
	}

	// Execute move task
	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(GetAvatarActorFromActorInfo());
	ARGX_MeleeAngel* MeleeAngel = Cast<ARGX_MeleeAngel>(Enemy);
	if (MeleeAngel)
	{
		MeleeAngel->bFlying = true;
	}

	const FVector CurrentLocation = Enemy->GetActorLocation();
	FVector TargetLocation = Enemy->TargetActor->GetActorLocation();

	const FRotator RotationOffset = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	Enemy->SetActorRotation(RotationOffset);

	const FVector EnemyForward = Enemy->GetActorForwardVector();
	const FVector RaySrc = CurrentLocation + EnemyForward * 50.0f;
	const FVector RayEnd = CurrentLocation + EnemyForward * 10000.0f;
	FHitResult RayTraceResult;

	TargetLocation = CurrentLocation + EnemyForward * 10000.0f;

	if (GetWorld()->LineTraceSingleByChannel(RayTraceResult, RaySrc, RayEnd, ECollisionChannel::ECC_WorldStatic))
	{
		if (RayTraceResult.ImpactPoint != FVector(0.0, 0.0, 0.0))
		{
			TargetLocation = RayTraceResult.ImpactPoint;
		}
	}

	UKismetSystemLibrary::DrawDebugLine(GetWorld(), RaySrc, RayEnd, FColor(255, 0, 0), 5.0, 5.0f);
	UKismetSystemLibrary::DrawDebugPoint(GetWorld(), TargetLocation, 100.0f, FColor(255, 255, 255), 5.0f);

	// Duration of the charge based on distance and speed
	const FVector VectorToTarget = TargetLocation - CurrentLocation;
	const float DistanceToTarget = VectorToTarget.Size();
	const float Duration = DistanceToTarget / ChargeSpeed;

	// TODO: Change this task for a task that follows the player until collision
	URGX_MoveToLocationCharge* MoveToLocationTask = URGX_MoveToLocationCharge::MoveToLocationCharge(this, NAME_None, TargetLocation, Duration, nullptr, nullptr);
	MoveToLocationTask->OnTargetLocationReached.AddDynamic(this, &URGX_MEAChargeAbility::OnDestinationReached);
	//UAbilityTask_MoveToLocation* MoveToLocationTask = UAbilityTask_MoveToLocation::MoveToLocation(this, NAME_None, TargetLocation, 5.0f, nullptr, nullptr);
	MoveToLocationTask->ReadyForActivation();
}

void URGX_MEAChargeAbility::OnDestinationReached()
{
	UE_LOG(LogTemp, Warning, TEXT("Destination Reached\n"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_MEAChargeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ARGX_MeleeAngel* MEAngel = Cast<ARGX_MeleeAngel>(GetAvatarActorFromActorInfo());
	if (MEAngel)
	{
		MEAngel->SetGravityScale(1.0);
		MEAngel->bFlying = false;
		MEAngel->bCharged = false;
		FRotator Rotation = MEAngel->GetActorRotation();
		Rotation.Pitch = 0.0;
		MEAngel->SetActorRotation(Rotation);
	}

	// Clean hitbox state
	URGX_HitboxesManagerComponent* HitboxManagerComponent = ActorInfo->AvatarActor->FindComponentByClass<URGX_HitboxesManagerComponent>();
	URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);
	if (Hitbox)
	{
		Hitbox->RemoveAbilityEffectsInfo();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
