
#include "RGX_LoadLevelTrigger.h"

#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"

#include "RegicideX/Actors/RGX_Arena.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/GameplayFramework/RGX_GameInstance.h"

// Sets default values
ARGX_LoadLevelTrigger::ARGX_LoadLevelTrigger()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);

	Portal = CreateDefaultSubobject<UChildActorComponent>(TEXT("Portal"));
	Portal->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARGX_LoadLevelTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void ARGX_LoadLevelTrigger::OnArenaDeactivated()
{
	Box->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ARGX_LoadLevelTrigger::OnPortalOverlapped, "OnPortalOverlapped");
}

void ARGX_LoadLevelTrigger::OnTeleportDone()
{
	if (URGX_GameInstance* gameInstance = Cast<URGX_GameInstance>(GetGameInstance()))
	{
		gameInstance->HideLoadingScreen();
	}
}

void ARGX_LoadLevelTrigger::UnloadLevels()
{
	if (URGX_GameInstance* gameInstance = Cast<URGX_GameInstance>(GetGameInstance()))
	{
		FLatentActionInfo LatentInfo;
		for (const FName& levelName : LevelsToUnload)
		{
			if (ULevelStreaming* levelStreamingInfo = UGameplayStatics::GetStreamingLevel(GetWorld(), levelName))
			{
				UGameplayStatics::UnloadStreamLevel(GetWorld(), levelName, LatentInfo, true);
				++LatentInfo.UUID;
			}
		}
	}
}

void ARGX_LoadLevelTrigger::OnPortalOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARGX_PlayerCharacter* player = Cast<ARGX_PlayerCharacter>(OtherActor);
	if (player == nullptr)
		return;

	if (URGX_GameInstance* gameInstance = Cast<URGX_GameInstance>(GetGameInstance()))
	{
		gameInstance->ShowLoadingScreen();

		FLatentActionInfo LatentInfo;
		for (const FName& levelName : LevelsToLoad)
		{
			if (ULevelStreaming* levelStreamingInfo = UGameplayStatics::GetStreamingLevel(GetWorld(), levelName))
			{
				++PendingLevelsToLoad;
				levelStreamingInfo->OnLevelShown.__Internal_AddDynamic(this, &ARGX_LoadLevelTrigger::OnLevelShown, "OnLevelShown");
				UGameplayStatics::LoadStreamLevel(GetWorld(), levelName, true, false, LatentInfo);
				++LatentInfo.UUID;
			}
		}
	}
}

void ARGX_LoadLevelTrigger::OnLevelShown()
{
	--PendingLevelsToLoad;

	if (PendingLevelsToLoad == 0)
		OnLevelsLoaded();
}