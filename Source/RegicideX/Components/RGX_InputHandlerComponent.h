// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RegicideX/Enums/RGX_InputEnums.h"
#include "RGX_InputHandlerComponent.generated.h"

USTRUCT()
struct FRGX_InputToken
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere)
	ERGX_PlayerInputID InputID;

	UPROPERTY(EditAnywhere)
	ERGX_InputProcessMode ProcessMode;

	UPROPERTY(EditAnywhere)
	bool bPressedInAir = false;

	UPROPERTY(EditAnywhere)
	float HoldTime;

	UPROPERTY(EditAnywhere)
	bool bRelease = false; // TODO: Right now this does nothing
};

USTRUCT()
struct FRGX_InputChainInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FRGX_InputToken> Inputs;

	UPROPERTY(EditAnywhere)
	bool bConsumeInput = false;

	UPROPERTY(EditAnywhere)
	bool bOnceInAir = true;

	bool bUsed = false;
};

USTRUCT()
struct FRGX_InputInfo
{
	GENERATED_BODY()

public:
	float HoldTime = 0.0f;
	bool bPressedInAir = false;
	bool bConsumed = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REGICIDEX_API URGX_InputHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URGX_InputHandlerComponent();

protected:
	void BeginPlay() override;

public:	
	void HandleInput(const ERGX_PlayerInputID NewInputID, const bool bRelease, const bool bIsInAir);
	void ResetAirState();

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	

private:
	bool ProcessInput(FRGX_InputToken& InputToken, float DeltaTime);

	bool ProcessInputToken(FRGX_InputToken& InputToken, float DeltaTime);

	bool IsInputPressed(const ERGX_PlayerInputID InputID);
	bool IsInputPressed(const uint16 InputID);
	
	void ConsumeInputChain(FRGX_InputChainInfo& InputChain);
	void ConsumeInput(const uint16 InputID);

protected:
	uint16 InputPressedState = 0x0000;
	
	UPROPERTY()
	TMap<uint16, FRGX_InputInfo> InputToInfoMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<ERGX_PlayerActions, FRGX_InputChainInfo> InputToActionMap;
};
