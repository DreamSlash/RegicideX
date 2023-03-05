// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GameplayFramework/RGX_LinearArenasGameMode.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequencePlayer.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Actors/RGX_Arena.h"
#include "RegicideX/Data/RGX_EnemiesDataTable.h"

#include "RegicideX/GameplayFramework/RGX_GameInstance.h"

void ARGX_LinearArenasGameMode::StartPlay()
{
	Super::StartPlay();
}

void ARGX_LinearArenasGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARGX_Arena::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		ARGX_Arena* Arena = Cast<ARGX_Arena>(Actor);
		if (Arena)
		{
			Arenas.Add(Arena);
			Arena->OnArenaActivated.AddDynamic(this, &ARGX_LinearArenasGameMode::OnArenaActivated);
			Arena->OnArenaDeactivated.AddDynamic(this, &ARGX_LinearArenasGameMode::OnArenaDeactivated);
			Arena->OnArenaEnemyKilled.AddDynamic(this, &ARGX_LinearArenasGameMode::OnArenaEnemyKilled);
			Arena->OnWaveSpawned.AddDynamic(this, &ARGX_LinearArenasGameMode::OnWaveSpawned);
			Arena->OnWaveFinished.AddDynamic(this, &ARGX_LinearArenasGameMode::OnWaveFinished);
		}
	}
}

void ARGX_LinearArenasGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (ARGX_Arena* Arena : Arenas)
	{
		Arena->OnArenaActivated.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnArenaActivated);
		Arena->OnArenaDeactivated.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnArenaDeactivated);
		Arena->OnArenaEnemyKilled.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnArenaEnemyKilled);
		Arena->OnWaveSpawned.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnWaveSpawned);
		Arena->OnWaveFinished.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnWaveFinished);
	}
}

void ARGX_LinearArenasGameMode::OnArenaActivated(ARGX_Arena* ActivatedArena)
{
	CurrentArena = ActivatedArena;
	BP_OnArenaActivated(ActivatedArena);
}

void ARGX_LinearArenasGameMode::OnArenaDeactivated(ARGX_Arena* DeactivatedArena)
{
	CurrentArena = nullptr;
	BP_OnArenaDeactivated(DeactivatedArena);

	if (DeactivatedArena->GetIsFinalArena() == true)
	{
		FTimerHandle CreditsTimer;
		GetWorldTimerManager().SetTimer(CreditsTimer, this, &ARGX_LinearArenasGameMode::FinalArenaFinished, 3.0f, false);
	}
}

void ARGX_LinearArenasGameMode::OnArenaEnemyKilled(ARGX_EnemyBase* EnemyKilled)
{
	UE_LOG(LogTemp, Warning, TEXT("On Arena Enemy Killed"));
	ERGX_EnemyType EnemyType = EnemyKilled->GetEnemyType();

	// TODO: Use map instead of table?
	FName RowName = UEnum::GetValueAsName(EnemyType);
	FRGX_EnemyTypeDataTable* EnemyTypeRow = DT_EnemyTypesInfo->FindRow<FRGX_EnemyTypeDataTable>(RowName, "");
	if (EnemyTypeRow)
	{
		const int ScoreToAdd = EnemyTypeRow->BaseScore;
		Score += ScoreToAdd;
		BP_OnArenaEnemyKilled();
	}
}

void ARGX_LinearArenasGameMode::OnWaveSpawned(URGX_OngoingWave* SpawnedWave)
{
	BP_OnWaveSpawned(SpawnedWave);
}

void ARGX_LinearArenasGameMode::OnWaveFinished(URGX_OngoingWave* FinishedWave)
{
	BP_OnWaveFinished(FinishedWave);
}

void ARGX_LinearArenasGameMode::TriggerCredits()
{
	UWorld* world = GetWorld();
	UWidgetLayoutLibrary::RemoveAllWidgets(world);
	currentLevelSequencePlayer = nullptr;

	UUserWidget* CreditsWidget = UUserWidget::CreateWidgetInstance(*world, CreditsWidgetClass, FName("CreditsWidget"));

	CreditsWidget->AddToViewport();
}


void ARGX_LinearArenasGameMode::FinalArenaFinished()
{
	UWorld* world = GetWorld();
	AActor* player = UGameplayStatics::GetPlayerCharacter(world, 0);
	check(player);
	APlayerController* player_controller = UGameplayStatics::GetPlayerController(world, 0);
	check(player_controller);

	player->SetHidden(true);
	player_controller->DisableInput(player_controller);

	// Reset music, logic in blueprint ... 
	URGX_GameInstance* game_instance = Cast<URGX_GameInstance>(UGameplayStatics::GetGameInstance(world));
	BP_OnPlayerWins();

	ALevelSequenceActor* levelSequenceActor = nullptr;
	FMovieSceneSequencePlaybackSettings settings;
	currentLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(world, EndingSequence, settings, levelSequenceActor);

	currentLevelSequencePlayer->OnFinished.AddDynamic(this, &ARGX_LinearArenasGameMode::TriggerCredits);
	currentLevelSequencePlayer->OnStop.AddDynamic(this, &ARGX_LinearArenasGameMode::TriggerCredits);

	currentLevelSequencePlayer->Play();

	UWidgetLayoutLibrary::RemoveAllWidgets(world);
}

void ARGX_LinearArenasGameMode::SkipCutscene()
{
	if (currentLevelSequencePlayer != nullptr && currentLevelSequencePlayer->IsPlaying())
	{
		currentLevelSequencePlayer->Stop();
	}
}
