#pragma once

#include "CoreMinimal.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"
#include "RGX_PeasantController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
class ARGX_Peasant;

UCLASS()
class REGICIDEX_API ARGX_PeasantController : public ARGX_EnemyBaseController
{
	GENERATED_BODY()
public:

	ARGX_PeasantController();

protected:
	virtual void OnPossess(APawn* pawn) override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(transient)
	UBlackboardComponent* BBComponent = nullptr;

	UPROPERTY(transient)
	UBehaviorTreeComponent* BTComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	ARGX_Peasant* Peasant = nullptr;

	// Blackboard key ids
	uint8 TargetActorID;
	uint8 DistanceToPlayerID;
	uint8 IdleActionID;
	bool bFrenzied = false;
};
