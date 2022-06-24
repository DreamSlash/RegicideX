// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/MeleeAngel/RGX_MEASweepAbility.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"
#include "RegicideX/GAS/AbilityTasks/RGX_AT_FollowActor.h"

bool URGX_MEASweepAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const 
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	ARGX_MeleeAngel* MEAngel = Cast<ARGX_MeleeAngel>(GetAvatarActorFromActorInfo());
	if (MEAngel == nullptr)
		return false;

	return true;
}

void URGX_MEASweepAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_MeleeAngel* MEAngel = Cast<ARGX_MeleeAngel>(GetAvatarActorFromActorInfo());
	AActor* ActorToFollow = MEAngel->TargetActor;

	URGX_AT_FollowActor* FollowActorTask = URGX_AT_FollowActor::FollowActor(this, NAME_None, ActorToFollow, 200.0f, 5.0f);
	FollowActorTask->OnFollowActorEnded.AddDynamic(this, &URGX_MEASweepAbility::OnEndSweepDuration);
	FollowActorTask->ReadyForActivation();
}

void URGX_MEASweepAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_MEASweepAbility::PopulateGameplayEffectContext(FRGX_GameplayEffectContext& GameplayEffectContext)
{
	float AbilityLevel = AbilityLevel = GetAbilityLevel();

	FString ContextString;
	FRealCurve* DamageCurve = DamageLevelCurve->FindCurve(DamageCurveName, ContextString);
	FRealCurve* ScalingCurve = DamageLevelCurve->FindCurve(AttributeScalingCurveName, ContextString);
	GameplayEffectContext.DamageAmount = DamageCurve->Eval(AbilityLevel);
	GameplayEffectContext.ScalingAttributeFactor = ScalingCurve->Eval(AbilityLevel);
}

void URGX_MEASweepAbility::OnEndSweepDuration()
{
	UE_LOG(LogTemp, Warning, TEXT("On End Sweep Duration\n"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
