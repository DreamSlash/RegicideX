
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RGX_ANS_CurveMovement.generated.h"

UCLASS()
class REGICIDEX_API URGX_ANS_CurveMovement : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseTick = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirMultiplier = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude = 0.0f;

public:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	float GetSpeed(USkeletalMeshComponent* MeshComp) const;

};
