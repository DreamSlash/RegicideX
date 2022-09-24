#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RGX_InteractInterface.generated.h"

UINTERFACE(BlueprintType)
class URGX_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

class IRGX_InteractInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AActor* ActorInteracting);
	virtual void Interact_Implementation(AActor* ActorInteracting) {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartCanInteract(AActor* ActorInteracting);
	virtual void StartCanInteract_Implementation(AActor* ActorInteracting) {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopCanInteract(AActor* ActorInteracting);
	virtual void StopCanInteract_Implementation(AActor* ActorInteracting) {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanBeInteractedWith(AActor* ActorInteracting);
	virtual bool CanBeInteractedWith_Implementation(AActor* ActorInteracting) { return true; };
};

DECLARE_LOG_CATEGORY_EXTERN(LogInteract, Log, All);