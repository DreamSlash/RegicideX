// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/Peasant/Suicidal/RGX_GA_SuicidalExplosionAbility.h"
#include "RGX_GA_SuicidalExplosionAbility.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

URGX_GA_SuicidalExplosionAbility::URGX_GA_SuicidalExplosionAbility()
{

}

bool URGX_GA_SuicidalExplosionAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
    if (bResult == false)
        return false;

    ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(ActorInfo->AvatarActor);
    if (Peasant == nullptr)
        return false;

    return true;
}

void URGX_GA_SuicidalExplosionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    UE_LOG(LogTemp, Warning, TEXT("Suicidal Explosion\n"));
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_GA_SuicidalExplosionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_GA_SuicidalExplosionAbility::OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
    Super::OnSuccessfulAbilityMontage(EventTag, EventData);
}

void URGX_GA_SuicidalExplosionAbility::OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
    Super::OnFailedAbilityMontage(EventTag, EventData);
}

void URGX_GA_SuicidalExplosionAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{

}
