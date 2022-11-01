#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RGX_EnemiesDataTable.generated.h"

UENUM(BlueprintType)
enum class ERGX_WaveSpawnMode : uint8
{
	Random							UMETA(DisplayName = "Random"),
	RoundRobin						UMETA(DisplayName = "RoundRobin")
};

USTRUCT(BlueprintType)
struct FRGX_EnemiesDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataAsset* EnemyInfo;
};

USTRUCT(BlueprintType)
struct FRGX_EnemyTypeDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERGX_EnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int BaseScore;
};

UCLASS(BlueprintType)
class URGX_ArenaWaveDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int> NumEnemies; // TODO: Enum instead of plain int

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URGX_ArenaWaveDataAsset* ChildWave;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnTimeDelay = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERGX_WaveSpawnMode WaveSpawnMode = ERGX_WaveSpawnMode::Random;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<int> SpawnerIdxAvailable; // Wave will spawn in spawners with those indices if they are available. If array is empty it will spawn in all spawners available

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WaveIntensity = 0.0f;
};