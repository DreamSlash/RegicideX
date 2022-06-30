// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/DistanceAngel/RGX_GA_BulletHell.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RegicideX/GAS/AbilityTasks/RGX_AT_WaitDelayAndSpawn.h"
#include "RegicideX/Actors/Weapons/RGX_ClusteredBullet.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"

bool URGX_GA_BulletHell::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(GetAvatarActorFromActorInfo());
	if (Enemy == nullptr)
		return false;

	return true;
}

void URGX_GA_BulletHell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Bullet Hell\n"));
	BulletOrigins = GetAvatarActorFromActorInfo()->GetComponentsByTag(USceneComponent::StaticClass(), FName("BulletHell"));
	if (BulletOrigins.Num() < 1)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}

	ARGX_EnemyBase* EnemyBase = Cast<ARGX_EnemyBase>(GetAvatarActorFromActorInfo());
	TargetActor = EnemyBase->TargetActor;

	URGX_AT_WaitDelayAndSpawn* WaitDelayAndSpawnTask = URGX_AT_WaitDelayAndSpawn::WaitDelayAndSpawn(this, FireTime, FireRate);
	WaitDelayAndSpawnTask->OnSpawn.AddDynamic(this, &URGX_GA_BulletHell::OnSpawnBullet);
	WaitDelayAndSpawnTask->OnFinish.AddDynamic(this, &URGX_GA_BulletHell::OnFinished);
	WaitDelayAndSpawnTask->ReadyForActivation();
}

void URGX_GA_BulletHell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_GA_BulletHell::OnSpawnBullet()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Bullet\n"));

	const FVector TargetLocation = TargetActor->GetActorLocation();

	for (const UActorComponent* ActorOrigin : BulletOrigins)
	{
		const USceneComponent* SceneOrigin = Cast<USceneComponent>(ActorOrigin);
		const FVector BulletOrigin = SceneOrigin->GetComponentLocation();
		const FRotator OriginRotation = SceneOrigin->GetComponentRotation();

		const FVector BulletDestination = BulletOrigin + SceneOrigin->GetForwardVector() * 1000.0f;
		//UKismetSystemLibrary::DrawDebugLine(GetWorld(), BulletOrigin, BulletDestination, FColor(255, 0, 0), 5.0, 5.0f);

		FVector OriginToTarget = TargetLocation - BulletOrigin;
		OriginToTarget.Normalize();

		FRotator BulletRotation = OriginRotation;
		BulletRotation.Pitch = OriginToTarget.Rotation().Pitch + FMath::FRandRange(-LowerBoundPitchOffset, UpperBoundPitchOffset);

		ARGX_ClusteredBullet* Bullet = GetWorld()->SpawnActor<ARGX_ClusteredBullet>(BulletBP, BulletOrigin, BulletRotation);
	}
}

void URGX_GA_BulletHell::OnFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
