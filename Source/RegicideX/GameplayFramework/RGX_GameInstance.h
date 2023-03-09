// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGX_GameInstance.generated.h"

class UFMODEvent;
struct FFMODEventInstance;

UCLASS()
class REGICIDEX_API URGX_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void StartGameInstance() override;

	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen();

	UFUNCTION(BlueprintCallable)
	void HideLoadingScreen();

	UFUNCTION(BlueprintCallable)
	void PlayMusic(UFMODEvent* MusicEvent);

	UFUNCTION(BlueprintCallable)
	void ResetMusic();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loading Screen")
	TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* LoadingWidget = nullptr;

	TSharedPtr<FFMODEventInstance> CurrentMusicEvent;
};
