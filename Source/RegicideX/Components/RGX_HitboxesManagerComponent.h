#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "GameplayEffect.h"
#include "RGX_HitboxesManagerComponent.generated.h"

class URGX_HitboxComponent;

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class URGX_HitboxesManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URGX_HitboxesManagerComponent();

	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void AddHitboxToMap(FGameplayTag IdentifierTag, URGX_HitboxComponent* HitboxComponent);

	UFUNCTION(BlueprintPure)
	URGX_HitboxComponent* GetHitboxByTag(FGameplayTag IdentifierTag);

protected:
	UPROPERTY()
	TMap<FGameplayTag, URGX_HitboxComponent*> HitboxesMap;
};