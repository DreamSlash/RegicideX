// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGX_GameInstance.generated.h"

UCLASS()
class REGICIDEX_API URGX_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen();

	UFUNCTION(BlueprintCallable)
	void HideLoadingScreen();

	UFUNCTION()
	void StartGameInstance() override;

	void ResetMusic();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loading Screen")
	TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* LoadingWidget = nullptr;

};
