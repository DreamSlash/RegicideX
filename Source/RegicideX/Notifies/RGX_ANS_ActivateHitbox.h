
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTags.h"
#include "RGX_ANS_ActivateHitbox.generated.h"

UCLASS()
class REGICIDEX_API URGX_ANS_ActivateHitbox : public UAnimNotifyState
{
	GENERATED_BODY()
	
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag HitboxTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EventTag;
};
