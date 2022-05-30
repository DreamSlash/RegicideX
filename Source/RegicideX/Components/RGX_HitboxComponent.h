#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
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

UENUM(BlueprintType)
enum class ERGX_DestroyOnOverlapType : uint8
{
	None				UMETA(DisplayName = "None"),	// Never destroyed
	Overlap				UMETA(DisplayName = "Overlap"),	// Destroy when overlapping anything
	Hostile				UMETA(DisplayName = "Hostile"),	// Destroy when overlapping enemies
	Dynamic				UMETA(DisplayName = "Dynamic")	// Destroy when overlapping any dynamics
};

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class URGX_HitboxComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	URGX_HitboxComponent();

	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void ActivateHitbox();

	UFUNCTION(BlueprintCallable)
	void DeactivateHitbox();
	
	UFUNCTION(BlueprintCallable)
	void ActivateEffect();

	UFUNCTION(BlueprintCallable)
	void DeactivateEffect();

	UFUNCTION()
	void SetAbilityEffectsInfo(const FRGX_AbilityEffectsInfo& NewAbilityEffectsInfo);

	UFUNCTION()
	void RemoveAbilityEffectsInfo();

	/* Check if the collider is going to hit the actor in the next frames taking into account 
		its velocity and position of both actors*/
	UFUNCTION()
	bool IsGoingToOverlapActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void SetChildActorAndSocket(UChildActorComponent* NewChildActorComponent, const FName NewSocketName);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasChildActor();

protected:

	void ApplyEffects(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestroyOwnerOnOverlap();

protected:

	UPROPERTY()
	TArray<AActor*> ActorsHit;

	UPROPERTY()
	TArray<UShapeComponent*> Shapes;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	TSubclassOf<UGameplayEffect> DefaultEffectToApply;

	UPROPERTY()
	TArray<FRGX_HitboxGameplayEvent> DefaultEventsToApply;

	UPROPERTY()
	FRGX_AbilityEffectsInfo AbilityEffectsInfo;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	TEnumAsByte<ETeamAttitude::Type> TeamToApply = ETeamAttitude::Hostile;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	bool bStartActive = true;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	bool bIsStatic = true;

	UPROPERTY()
	bool bEffectActivated = false;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	TEnumAsByte<ERGX_DestroyOnOverlapType> DestroyOnOverlap = ERGX_DestroyOnOverlapType::None;

	//UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	//bool bCanReceiveEffects = true;

	UChildActorComponent* ChildActorComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HitboxComponent)
	FName SocketName;

	/* Only used if it has socket*/
	FVector LastSocketPosition;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	float CastSphereRadius = 22.0f;	

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	TEnumAsByte<EObjectTypeQuery> TargetObjectType;
};