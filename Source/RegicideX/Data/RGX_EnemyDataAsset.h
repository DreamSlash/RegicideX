#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RGX_EnemyDataAsset.generated.h"

UENUM(BlueprintType)
enum class ERGX_EnemyTypes : uint8
{
	MeleePeasant		UMETA(DisplayName = "MeleePeasant"),
	DistancePeasant		UMETA(DisplayName = "DistancePeasant"),
	MolePeasant			UMETA(DisplayName = "MolePeasant"),
	SuicidePeasant		UMETA(DisplayName = "SuicidePeasant")
};

/**
 * Enemy DataAsset. It contains all the metadata of an enemy
 */
UCLASS()
class REGICIDEX_API URGX_EnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	URGX_EnemyDataAsset();
	~URGX_EnemyDataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ARGX_EnemyBase* EnemyClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<ARGX_EnemyBase> EnemyBP;

	UPROPERTY(EditAnywhere)
	ERGX_EnemyTypes TypeName;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int Life;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int Damage;
};
