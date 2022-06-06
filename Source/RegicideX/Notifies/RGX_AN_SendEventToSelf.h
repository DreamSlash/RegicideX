// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTags.h"
#include "RGX_AN_SendEventToSelf.generated.h"

class URGX_RGXEventDataAsset;
/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_AN_SendEventToSelf : public UAnimNotify
{
	GENERATED_BODY()
	
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URGX_RGXEventDataAsset* EventPayload;
};
