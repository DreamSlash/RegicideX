#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RegicideX/Components/RGX_ComboSystemComponent.h"
#include "RGX_ANS_JumpComboSection.generated.h"

UCLASS()
class REGICIDEX_API URGX_ANS_JumpComboSection : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	/** Section name to jump when appropiate input recevied. */
	UPROPERTY(EditAnywhere)
	FName SectionName;

	/** Input ID needed to be pressed to carry on with the combo. */
	UPROPERTY(EditAnywhere)
	ERGX_ComboTokenID InputID;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	bool bContinueCombo = false;
};
