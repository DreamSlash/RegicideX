#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RGX_InteractInterface.generated.h"

UINTERFACE(Blueprintable)
class URGX_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

class IRGX_InteractInterface
{
	GENERATED_BODY()
public:
	virtual void Interact(AActor* ActorInteracting) { }
	virtual void StartCanInteract(AActor* ActorInteracting) { }
	virtual void StopCanInteract(AActor* ActorInteracting) { }
};

DECLARE_LOG_CATEGORY_EXTERN(LogInteract, Log, All);