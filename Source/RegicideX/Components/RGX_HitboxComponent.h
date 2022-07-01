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

DECLARE_DELEGATE_OneParam(FOnTargetOverlapDelegate, AActor*)

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

	UFUNCTION()
	void SetGameplayEffectContextHandle(FGameplayEffectContextHandle Handle);

	/* Check if the collider is going to hit the actor in the next frames taking into account 
		its velocity and position of both actors*/
	UFUNCTION()
	bool IsGoingToOverlapActor(AActor* Actor);

public:
	FOnTargetOverlapDelegate OnTargetOverlap;

protected:

	void ApplyEffects(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


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
	FGameplayEffectContextHandle DefaultGameplayEffectContextHandle;

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

	/** "This tags do not allow the hitbox effect to get applied to the actor if owned by him." */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HitboxComponent)
	FGameplayTagContainer TagsToBlockTheHit;

	/** "This enum property defines if the component is destroyed OnOverlap by the type.*/
	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	TEnumAsByte<ERGX_DestroyOnOverlapType> DestroyOnOverlap = ERGX_DestroyOnOverlapType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HitboxComponent)
	FName SocketName;

	/* Only used if it has socket*/
	FVector LastSocketPosition;

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	float CastSphereRadius = 22.0f;	

	UPROPERTY(EditDefaultsOnly, Category = HitboxComponent)
	TEnumAsByte<EObjectTypeQuery> TargetObjectType;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCameraShakeBase> CameraShake;

private:
	bool CheckIfEffectIsApplied(AActor* TargetActor);

	void ResetCustomTimeDilation();

	AActor* Owner = nullptr;
	TArray<AActor*> ActorsWithTimeDilation;
	FTimerHandle PunchTimerHandle;
};

DECLARE_LOG_CATEGORY_EXTERN(LogHitbox, Log, All);