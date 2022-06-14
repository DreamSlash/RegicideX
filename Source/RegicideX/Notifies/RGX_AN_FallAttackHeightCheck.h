// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RGX_AN_FallAttackHeightCheck.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_AN_FallAttackHeightCheck : public UAnimNotify
{
	GENERATED_BODY()
	
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
};
