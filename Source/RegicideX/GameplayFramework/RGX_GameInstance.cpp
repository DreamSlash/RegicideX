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

void URGX_GameInstance::BeginLoadingScreen(const FString& MapName)
{
	if(!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;

		LoadingWidget = CreateWidget<UUserWidget>(this, WidgetClass);
		TSharedPtr<SWidget> WidgetPtr = LoadingWidget->TakeWidget();
		LoadingScreen.WidgetLoadingScreen = WidgetPtr;

		// Play Movies Setting
		/*
		* LoadingScreen.MoviePaths.Add("movie_name")M
		* LoadingScreen.bMoviesAreSkippable = true;
		* LoadingScreen.bWaitForManualStop = true;
		* LoadingScreen.PlaybackType = EMoviePlaybackType::MT_Looped;
		*/

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void URGX_GameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	if (!IsRunningDedicatedServer())
	{
		if (LoadingWidget)
		{
			LoadingWidget->RemoveFromParent();
			LoadingWidget->MarkPendingKill();
		}
	}
}
