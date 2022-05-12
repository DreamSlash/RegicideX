#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "GameplayEffect.h"
#include "GenericTeamAgentInterface.h"
#include "RGX_HitboxComponent.generated.h"

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

protected:

	void ApplyEffects(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectToApply;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETeamAttitude::Type> TeamToApply = ETeamAttitude::Hostile;

	UPROPERTY(EditDefaultsOnly)
	bool bStartActive = true;

	UPROPERTY()
	bool bEffectActivated = false;
};