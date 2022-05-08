#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

#include "RGX_EnemyDataAsset.generated.h"

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
	
};
