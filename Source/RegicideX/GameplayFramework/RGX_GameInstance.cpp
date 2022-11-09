// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GameplayFramework/RGX_GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MoviePlayer.h"
#include "GameFramework/GameUserSettings.h"

void URGX_GameInstance::StartGameInstance()
{
	Super::StartGameInstance();

	UGameUserSettings* Settings = GetEngine()->GetGameUserSettings();
	Settings->SetFullscreenMode(EWindowMode::Fullscreen);
	Settings->SetScreenResolution(FIntPoint(1920, 1080));
	Settings->ApplySettings(true);
}

void URGX_GameInstance::BeginLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	if(!IsRunningDedicatedServer())
	{
		LoadingWidget = CreateWidget<UUserWidget>(this, WidgetClass);
		TSharedPtr<SWidget> WidgetPtr = LoadingWidget->TakeWidget();

		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.WidgetLoadingScreen = WidgetPtr;
		LoadingScreen.bWaitForManualStop = true;
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void URGX_GameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{ 
	GetMoviePlayer()->StopMovie();
}
