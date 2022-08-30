
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RGX_PlayerCharacterController.generated.h"

UCLASS()
class REGICIDEX_API ARGX_PlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	void PlayerTick(float DeltaTime) override;
	void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

public:
	// TODO: Make delegates in GameplayAbilities?
	void OnStartFallAttack();
	void OnEndFallAttack();
};
