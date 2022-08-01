// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/Peasant/Suicidal/RGX_GA_SuicidalExplosionAbility.h"
#include "RGX_GA_SuicidalExplosionAbility.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

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
	if (EventTag.MatchesTagExact(JumpTag))
	{
		AActor* OwnerActor = GetOwningActorFromActorInfo();
		ARGX_CharacterBase* OwnerCharacter = Cast<ARGX_CharacterBase>(OwnerActor);
		if (OwnerCharacter)
		{
			FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(*JumpEffect, OwnerCharacter->GetCharacterLevel());
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
		}
	}

	if (EventTag.MatchesTagExact(ExplosionTag))
	{
		Explode();
	}
}

void URGX_GA_SuicidalExplosionAbility::Explode()
{
	const FVector SpawnLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
	const FRotator SpawnRotation = FRotator(0.0f);

	//AActor* SpawnedBarrier = GetWorld()->SpawnActor<AActor>(BarrierClass, SpawnLocation, SpawnRotation);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	//UClass* SeekClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SpawnLocation, ExplosionRadius, TraceObjectTypes, nullptr, IgnoreActors, OutActors);

	for (AActor* Actor : OutActors)
	{
		ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(Actor);
		if (Enemy == nullptr)
		{
			ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(Actor);
			if (PlayerCharacter == nullptr)
				continue;
		}

		const FVector TargetLocation = Actor->GetActorLocation();
		FVector DirectionVector = TargetLocation - SpawnLocation;
		DirectionVector.Normalize();

		const FVector ArrowVector = DirectionVector * 500.0f * FVector(0.0f, 0.0f, 200.0f);

		const FVector LineStart = TargetLocation;
		const FVector LineEnd = TargetLocation + ArrowVector;

		const FLinearColor ArrowColor = FLinearColor::Red;

		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), LineStart, LineEnd, 10.0f, ArrowColor, 3.0f, 2.0f);

		FGameplayEventData EventPayload;
		EventPayload.Instigator = CurrentActorInfo->AvatarActor.Get();
		EventPayload.OptionalObject = LaunchEventData;

		UAbilitySystemComponent* SourceACS = GetAbilitySystemComponentFromActorInfo();
		UAbilitySystemComponent* TargetACS = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
		if (TargetACS)
		{
			FGameplayEffectContextHandle ContextHandle = SourceACS->MakeEffectContext();
			FRGX_GameplayEffectContext* FRGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());
			FRGXContext->AddInstigator(CurrentActorInfo->AvatarActor.Get(), CurrentActorInfo->AvatarActor.Get());
			FRGXContext->DamageAmount = ExplosionDamage;
			FRGXContext->ScalingAttributeFactor = 1.0f;

			TargetACS->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Launched")), &EventPayload);
			SourceACS->ApplyGameplayEffectToTarget(EffectToApply->GetDefaultObject<UGameplayEffect>(), TargetACS, 1.0f, ContextHandle);
		}
	}

	UGameplayStatics::SpawnEmitterAtLocation(CurrentActorInfo->AvatarActor->GetWorld(), ExplosionVFX, CurrentActorInfo->AvatarActor->GetTransform());

	FGameplayEventData DeadEventPayload;
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(InstantDeathEffect->GetDefaultObject<UGameplayEffect>(), 1, GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());
}
