#include "RGX_InputEnums.generated.h"

UENUM(BlueprintType)
enum class ERGXPlayerInputID : uint8
{
	None							UMETA(DisplayName = "None"),
	LightAttackInput				UMETA(DisplayName = "LightAttackInput"),
	SpecialLightAttackInput			UMETA(DisplayName = "SpecialLightAttackInput"),
	HeavyAttackInput				UMETA(DisplayName = "HeavyAttackInput"),
	HeavyAttackHoldInput			UMETA(DisplayName = "HeavyAttackHoldInput")
};