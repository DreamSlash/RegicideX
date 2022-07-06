#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "GameplayEffect.h"
#include "GenericTeamAgentInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RGX_HitboxComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRGX_HitboxOverlapDelegate, class AActor*, HitActor);

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
	Dynamic				UMETA(DisplayName = "Dynamic"),	// Destroy when overlapping any dynamics
	EffectApplied		UMETA(DisplayName = "EffectApplied")	// Destroy when overlapping and bind effect is applied.
};

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class URGX_HitboxComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	URGX_HitboxComponent();

	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Activate hitbox for detecting overlaps and set collision preset to dodgeable. */
	UFUNCTION(BlueprintCallable)
	void ActivateHitbox();

	/** Deactivate hitbox. */
	UFUNCTION(BlueprintCallable)
	void DeactivateHitbox();

	/** Add the tag to the list to activate */
	UFUNCTION()
	void AddEventTag(const FGameplayTag& NewTag);

	/** Remove the tag from the list to activate */
	UFUNCTION()
	void RemoveEventTag(const FGameplayTag& NewTag);

	/* Check if the collider is going to hit the actor in the next frames taking into account 
		its velocity and position of both actors*/
	UFUNCTION()
	bool IsGoingToOverlapActor(AActor* Actor);

public:
	FRGX_HitboxOverlapDelegate OnHitboxOverlap;

protected:

	//void ApplyEffects(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void OnComponentOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

	void DestroyOwnerOnOverlap();

protected:

	/** Array to store all actors hit by the Hitbox. */
	UPROPERTY()
	TArray<AActor*> ActorsHit;

	/** All shapes forming the hitbox component. */
	UPROPERTY()
	TArray<UShapeComponent*> Shapes;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	TEnumAsByte<ETeamAttitude::Type> TeamToApply = ETeamAttitude::Hostile;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	bool bStartActive = true;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	bool bIsStatic = true;

	UPROPERTY()
	bool bEffectActivated = false;

	/** Map of gameplay tags to trigger events. */
	UPROPERTY(/*EditDefaultsOnly, BlueprintReadOnly, Category = HitboxComponent*/)
	TArray<FGameplayTag> EffectTags;

	/** "This tags do not allow the hitbox effect to get applied to the actor if owned by him." */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HitboxComponent)
	FGameplayTagContainer TagsToBlockTheHit;

	/** "This enum property defines if the component is destroyed OnOverlap by the type.*/
	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	TEnumAsByte<ERGX_DestroyOnOverlapType> DestroyOnOverlap = ERGX_DestroyOnOverlapType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HitboxComponent)
	FName SocketName;

	/* Only used if it has socket. */
	FVector LastSocketPosition;

	/** Radius to check all actors around and if is going to overlap them. */
	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	float CastSphereRadius = 22.0f;	

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	TEnumAsByte<EObjectTypeQuery> TargetObjectType;

private:

	void ResetCustomTimeDilation();

	AActor* Owner = nullptr;
	TArray<AActor*> ActorsWithTimeDilation;
	FTimerHandle PunchTimerHandle;
};

DECLARE_LOG_CATEGORY_EXTERN(LogHitbox, Log, All);