#pragma once

#include "CoreMinimal.h"
#include "RGX_InteractComponent.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class URGX_InteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	//UPROPERTY(EditDefaultsOnly)
	//float InteractionDistance = 100.0f;

	UPROPERTY(BlueprintReadOnly, Transient)
	TWeakObjectPtr<AActor> CurrentActor = nullptr;

	// The collision channel used when query the world for object to interact with
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	//TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ZOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EObjectTypeQuery> InteractableObjectType;

	URGX_InteractComponent();

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TryToInteract();
};