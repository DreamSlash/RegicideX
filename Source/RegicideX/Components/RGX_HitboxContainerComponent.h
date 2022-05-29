#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "GameplayEffect.h"
#include "GenericTeamAgentInterface.h"
#include "RGX_HitboxContainerComponent.generated.h"

class UShapeComponent;

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class URGX_HitboxContainerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	URGX_HitboxContainerComponent();

	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void ActivateHitbox();
	UFUNCTION(BlueprintCallable)
	void DeactivateHitbox();

protected:

	void ApplyEffects(AActor* OtherActor);

	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	TArray<AActor*> ActorsHit;

	UPROPERTY()
	TArray<UShapeComponent*> Shapes;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApply;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETeamAttitude::Type> TeamToApply = ETeamAttitude::Hostile;

	UPROPERTY(EditDefaultsOnly)
	bool bStartActive = true;
};