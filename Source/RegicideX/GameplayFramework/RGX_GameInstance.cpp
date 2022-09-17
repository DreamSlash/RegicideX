// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GameplayFramework/RGX_GameInstance.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

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
