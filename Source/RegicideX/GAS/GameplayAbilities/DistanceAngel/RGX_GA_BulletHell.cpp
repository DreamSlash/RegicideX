// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/DistanceAngel/RGX_GA_BulletHell.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"

bool URGX_GA_BulletHell::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void URGX_GA_BulletHell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Bullet Hell\n"));

	TArray<UActorComponent*> BulletOrigins = GetAvatarActorFromActorInfo()->GetComponentsByTag(USceneComponent::StaticClass(), FName("BulletHell"));

	for (const UActorComponent* ActorOrigin : BulletOrigins)
	{
		const USceneComponent* SceneOrigin = Cast<USceneComponent>(ActorOrigin);
		const FVector BulletOrigin = SceneOrigin->GetComponentLocation();
		const FRotator BulletRotation = SceneOrigin->GetComponentRotation();

		const FVector BulletDestination = BulletOrigin + SceneOrigin->GetForwardVector() * 1000.0f;
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), BulletOrigin, BulletDestination, FColor(255, 0, 0), 5.0, 5.0f);
	}
}

void URGX_GA_BulletHell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
