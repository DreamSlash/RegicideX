#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "GameplayEffect.h"
#include "GenericTeamAgentInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "../GAS/RGX_PayloadObjects.h"
#include "RGX_HitboxComponent.generated.h"

USTRUCT()
struct FRGX_HitboxGameplayEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag GameplayEvent;
	
	UPROPERTY()
	FGameplayEventData EventData;

	UPROPERTY()
	bool bActivated = false;
};

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class URGX_HitboxComponent : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	URGX_HitboxComponent();

	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void ActivateHitbox();

	UFUNCTION(BlueprintCallable)
	void DeactivateHitbox();
	
	UFUNCTION(BlueprintCallable)
	void ActivateEffect();

	UFUNCTION(BlueprintCallable)
	void DeactivateEffect();

	UFUNCTION(BlueprintCallable)
	void ActivateEvent(const FGameplayTag& EventTag);

	UFUNCTION(BlueprintCallable)
	void DeactivateEvent(const FGameplayTag& EventTag);

	UFUNCTION(BlueprintCallable)
	void AddEvent(const FGameplayTag& EventTag, const FGameplayEventData& EventData, bool bAutoActivateEvent);

	UFUNCTION(BlueprintCallable)
	void RemoveEvent(const FGameplayTag& EventTag);

	UFUNCTION()
	void SetAbilityEffectsInfo(FRGX_AbilityEffectsInfo& NewAbilityEffectsInfo);

	UFUNCTION()
	void RemoveAbilityEffectsInfo();

protected:

	void ApplyEffects(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectToApply;

	UPROPERTY()
	TArray<FRGX_HitboxGameplayEvent> EventsToApply;

	UPROPERTY()
	FRGX_AbilityEffectsInfo AbilityEffectsInfo;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETeamAttitude::Type> TeamToApply = ETeamAttitude::Hostile;

	UPROPERTY(EditDefaultsOnly)
	bool bStartActive = true;

	UPROPERTY()
	bool bEffectActivated = false;
};