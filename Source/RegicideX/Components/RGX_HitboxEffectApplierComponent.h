#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RGX_HitboxEffectApplierComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class URGX_HitboxEffectApplierComponent : public URGX_HitboxComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRGX_EffectContextContainer> EffectToApplyToTargetWithPayload;

protected:
	void SendCollisionEvents(AActor* OwnerActor, AActor* OtherActor, bool bFromSweep, const FHitResult& SweepResult) override;

};