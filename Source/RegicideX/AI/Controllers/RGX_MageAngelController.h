// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/AI/Controllers/RGX_EnemyBaseController.h"
#include "RGX_MageAngelController.generated.h"

UENUM(BlueprintType)
namespace ERGX_MageAngelPhase
{
	enum Type
	{
		Phase1		UMETA(DisplayName = "Phase1"),
		Phase2		UMETA(DisplayName = "Phase2"),
		Phase3		UMETA(DisplayName = "Phase3"),
	};
}

UCLASS()
class REGICIDEX_API ARGX_MageAngelController : public ARGX_EnemyBaseController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* Phase1Tree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* Phase2Tree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* Phase3Tree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float Phase2Threshold = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float Phase3Threshold = 0.3f;

public:
	void OnMageAngelHealthChanged(float CurrentHealth, float MaxHealth);

protected:
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	void UpdateMageAngelPhase(ERGX_MageAngelPhase::Type Phase);

protected:
	ERGX_MageAngelPhase::Type MageAngelPhase = ERGX_MageAngelPhase::Phase1;

};
