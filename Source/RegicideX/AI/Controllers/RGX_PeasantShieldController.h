#pragma once

#include "CoreMinimal.h"
#include "RegicideX/AI/Controllers/RGX_PeasantController.h"
#include "RGX_PeasantShieldController.generated.h"

UCLASS()
class REGICIDEX_API ARGX_PeasantShieldController : public ARGX_PeasantController
{
	GENERATED_BODY()

public:
	ARGX_PeasantShieldController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* LogicAfterShieldBroken = nullptr;

public:
	void OnShieldBroken();

};
