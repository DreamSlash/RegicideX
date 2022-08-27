// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/Actors/Projectiles/RGX_HitboxProjectile.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"

ARGX_HitboxProjectile::ARGX_HitboxProjectile()
{

}

void ARGX_HitboxProjectile::BeginPlay()
{
	Super::BeginPlay();

	HitboxComponent->OnHitboxOverlap.AddDynamic(this, &ARGX_HitboxProjectile::OnHitboxOverlap);
}

void ARGX_HitboxProjectile::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	HitboxComponent->OnHitboxOverlap.RemoveDynamic(this, &ARGX_HitboxProjectile::OnHitboxOverlap);
}

void ARGX_HitboxProjectile::OnHitboxOverlap(AActor* OverlappedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile Overlap\n"));
	ARGX_CharacterBase* CharacterBaseInstigator = Cast<ARGX_CharacterBase>(Instigator);
	if (CharacterBaseInstigator == nullptr)
		return;

	UAbilitySystemComponent* SourceACS = Instigator->FindComponentByClass<UAbilitySystemComponent>();
	UAbilitySystemComponent* TargetACS = OverlappedActor->FindComponentByClass<UAbilitySystemComponent>();
	if (SourceACS && TargetACS)
	{
		FGameplayEffectContextHandle ContextHandle = SourceACS->MakeEffectContext();
		FRGX_GameplayEffectContext* RGXContext = static_cast<FRGX_GameplayEffectContext*>(ContextHandle.Get());

		for (FRGX_EffectContextContainer& EffectContextContainer : EffectContextsToApply)
		{
			if (ensureMsgf(EffectContextContainer.EffectToApply.Get(), TEXT("[Error] OnHitboxOverlap: %s Effect was nullptr"), *GetName()))
			{
				RGXContext->OptionalObject = EffectContextContainer.Payload;
				SourceACS->ApplyGameplayEffectToTarget(EffectContextContainer.EffectToApply->GetDefaultObject<UGameplayEffect>(), TargetACS, CharacterBaseInstigator->GetCharacterLevel(), ContextHandle);
			}
		}
	}
}
