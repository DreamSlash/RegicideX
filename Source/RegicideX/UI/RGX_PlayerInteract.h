// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RGX_PlayerInteract.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_PlayerInteract : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractionText;
};
