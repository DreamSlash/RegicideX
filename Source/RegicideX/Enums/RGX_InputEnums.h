#pragma once

#include "RGX_InputEnums.generated.h"

// TODO: To be deleted
UENUM(BlueprintType)
enum class ERGX_ComboTokenID : uint8
{
	None							UMETA(DisplayName = "None"),
	LightAttackToken				UMETA(DisplayName = "LightAttackToken"),
	LightAttackHoldToken			UMETA(DisplayName = "LightAttackHoldToken"),
	SpecialLightAttackToken			UMETA(DisplayName = "SpecialLightAttackToken"),
	HeavyAttackToken				UMETA(DisplayName = "HeavyAttackToken"),
	HeavyAttackHoldToken			UMETA(DisplayName = "HeavyAttackHoldToken")
};

UENUM()
enum class ERGX_PlayerInputID : uint16
{
	None				= 0x0000		UMETA(DisplayName = "None"),
	LightAttackInput	= 0x0002		UMETA(DisplayName = "LightAttackInput"),
	HeavyAttackInput	= 0x0004		UMETA(DisplayName = "HeavyAttackInput"),
	JumpInput			= 0x0008		UMETA(DisplayName = "JumpInput")
};

UENUM(BlueprintType)
enum class ERGX_InputProcessMode : uint8
{
	None			UMETA(DisplayName = "None"),
	Click			UMETA(DisplayName = "Click"),
	Hold			UMETA(DisplayName = "Hold"),
	DoubleClick		UMETA(DisplayName = "DoubleClick")
};

UENUM(BlueprintType)
enum class ERGX_PlayerActions : uint8
{
	None				UMETA(DisplayName = "None"),
	LaunchAttack		UMETA(DisplayName = "LaunchAttackAction"),
	FallAttack			UMETA(DisplayName = "FallAttackAction"),
	HeavyAttack			UMETA(DisplayName = "HeavyAttackAction")
};