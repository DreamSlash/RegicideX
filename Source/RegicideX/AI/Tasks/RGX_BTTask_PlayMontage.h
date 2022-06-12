// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BTTask_PlayMontage.generated.h"


UCLASS()
class REGICIDEX_API URGX_BTTask_PlayMontage : public UBTTaskNode
{

	GENERATED_BODY()

public:
	URGX_BTTask_PlayMontage();

	/** Animation asset to play. Note that it needs to match the skeleton of pawn this BT is controlling */
	UPROPERTY(Category = Node, EditAnywhere)
		UAnimMontage* MontageToPlay;

	/** if true the task will just trigger the animation and instantly finish. Fire and Forget. */
	UPROPERTY(Category = Node, EditAnywhere)
		bool bNonBlocking;

	UPROPERTY(Category = Node, EditAnywhere)
		FName NotifyEnd;

	UPROPERTY(Category = Node, EditAnywhere)
		float BlendOutTime = 0.2f;


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

protected:

	UFUNCTION()
	void OnReceiveNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void CleanUp();
	void StopMontage();

	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	TWeakObjectPtr<UAnimInstance> CachedAnimInstance;
	
};
