// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RGX_BTService_CalculateActorFuturePosition.generated.h"

UCLASS()
class REGICIDEX_API URGX_BTService_CalculateActorFuturePosition : public UBTService
{
	GENERATED_BODY()

public:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	UPROPERTY(EditAnywhere)
		float MaxReachTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector PositionKey;

private:
	const AActor* GetTargetActor(UBehaviorTreeComponent& OwnerComp) const;
	FVector CalculateFuturePosition(UBehaviorTreeComponent& OwnerComp, const AActor* Target) const;
	bool SetPositionValue(UBehaviorTreeComponent& OwnerComp, const FVector& Position);

};
