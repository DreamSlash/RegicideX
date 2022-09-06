#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RGX_PeasantController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class REGICIDEX_API ARGX_PeasantController : public AAIController
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

	// Blackboard key ids
	uint8 TargetActorID;
	uint8 DistanceToPlayerID;
	uint8 IdleActionID;
	bool bFrenzied = false;
};
