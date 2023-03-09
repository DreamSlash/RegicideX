// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GameplayFramework/RGX_GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
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

void URGX_GameInstance::ShowLoadingScreen()
{
	if (LoadingWidget == nullptr)
	{
		LoadingWidget = CreateWidget(this, WidgetClass, "LoadingScreen");
	}
		
	LoadingWidget->AddToViewport();
}

void URGX_GameInstance::HideLoadingScreen()
{
	if (LoadingWidget && LoadingWidget->IsInViewport())
	{
		LoadingWidget->RemoveFromParent();
	}
}

void URGX_GameInstance::PlayMusic(UFMODEvent* MusicEvent)
{
	if (MusicEvent)
	{
		ResetMusic();

		CurrentMusicEvent = MakeShared<FFMODEventInstance>(UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MusicEvent, true));
	}
}

void URGX_GameInstance::ResetMusic()
{
	if (CurrentMusicEvent)
	{
		UFMODBlueprintStatics::EventInstanceSetPaused(*CurrentMusicEvent, true);
		UFMODBlueprintStatics::EventInstanceStop(*CurrentMusicEvent, true);
	}
}
