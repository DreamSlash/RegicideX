
#include "RegicideX/Components/RGX_InputHandlerComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"

URGX_InputHandlerComponent::URGX_InputHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URGX_InputHandlerComponent::BeginPlay()
{
	Super::BeginPlay();	

	FRGX_InputInfo LightAttackInputInfo;
	InputToInfoMap.Emplace((uint16)ERGX_PlayerInputID::LightAttackInput, LightAttackInputInfo);

	FRGX_InputInfo HeavyAttackInputInfo;
	InputToInfoMap.Add((uint16)ERGX_PlayerInputID::HeavyAttackInput, HeavyAttackInputInfo);

	FRGX_InputInfo JumpInputInfo;
	InputToInfoMap.Add((uint16)ERGX_PlayerInputID::JumpInput, std::move(JumpInputInfo));
}

void URGX_InputHandlerComponent::HandleInput(ERGX_PlayerInputID NewInputID, const bool bRelease, const bool bIsInAir)
{
	if (bRelease == false)
	{
		InputPressedState = InputPressedState | (uint16)NewInputID;
		InputToInfoMap[(uint16)NewInputID].bPressedInAir = bIsInAir;
	}
	else
	{
		// Last change to perform an action in case the action needs an input release
		InputToInfoMap[(uint16)NewInputID].bReleased = true;
		UpdateInputActions();

		// Clean up the input info
		uint16 BitMask = ~(uint16)NewInputID;
		InputPressedState = InputPressedState & BitMask;
		InputToInfoMap[(uint16)NewInputID].bPressedInAir = false;
		InputToInfoMap[(uint16)NewInputID].bConsumed = false;
		InputToInfoMap[(uint16)NewInputID].bReleased = false;
	}
}

void URGX_InputHandlerComponent::ResetAirState()
{
	for (TPair<ERGX_PlayerActions, FRGX_InputChainInfo>& InputToAction : InputToActionMap)
	{
		InputToAction.Value.bUsed = false;
	}
}

void URGX_InputHandlerComponent::ResetInputState()
{
	InputPressedState = 0x0000;

	for (TPair<uint16, FRGX_InputInfo>& InputToHold : InputToInfoMap)
	{
		InputToHold.Value.HoldTime = 0.0f;
	}
}

void URGX_InputHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateInputInfo(DeltaTime);
	UpdateInputActions();
}

void URGX_InputHandlerComponent::UpdateInputInfo(float DeltaTime)
{
	// Add holding time for the specific input.
	for (TPair<uint16, FRGX_InputInfo>& InputToHold : InputToInfoMap)
	{
		if (IsInputPressed(InputToHold.Key) == true)
		{
			InputToHold.Value.HoldTime += DeltaTime;
			//UE_LOG(LogTemp, Warning, TEXT("%f\n"), InputToHold.Value.HoldTime);
		}
		else
		{
			InputToHold.Value.HoldTime = 0.0f;
		}
	}
}

void URGX_InputHandlerComponent::UpdateInputActions()
{
	/** Iterate through all possible actions and handle them if necessary. */
	for (TPair<ERGX_PlayerActions, FRGX_InputChainInfo>& InputToAction : InputToActionMap)
	{
		bool bResult = true;

		// Check if Input Token was really pressed on air, already used or if cannot be processed.
		// Otherwise, mark as true to handle the action.
		for (FRGX_InputToken InputToken : InputToAction.Value.Inputs)
		{
			if (InputToInfoMap[(uint16)InputToken.InputID].bPressedInAir != InputToken.bPressedInAir)
			{
				bResult = false;
				break;
			}

			if (InputToAction.Value.bUsed == true)
			{
				bResult = false;
				break;
			}

			if (ProcessInput(InputToken) == false)
			{
				bResult = false;
			}
		}

		if (bResult == true)
		{
			ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(GetOwner());
			PlayerCharacter->HandleAction(InputToAction.Key);

			if (InputToAction.Value.bOnceInAir == true)
			{
				InputToAction.Value.bUsed = true;
			}

			if (InputToAction.Value.bConsumeInput == true)
			{
				ConsumeInputChain(InputToAction.Value);
			}
		}
	}
}

bool URGX_InputHandlerComponent::ProcessInput(FRGX_InputToken& InputToken)
{
	bool bResult = true;

	//UE_LOG(LogTemp, Warning, TEXT("Input: %d\n"), (uint16)InputToken.InputID);
	//UE_LOG(LogTemp, Warning, TEXT("Input State: %d\n"), InputPressedState);

	if (InputToInfoMap[(uint16)InputToken.InputID].bConsumed == true)
		return false;

	if (IsInputPressed(InputToken.InputID))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input %d Pressed\n"), (uint16)InputToken.InputID);
		if (ProcessInputToken(InputToken) == false)
		{
			bResult = false;
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("NotPressed\n"));
		bResult = false;
	}

	return bResult;
}

// TODO [REFACTOR]: Each input process should have its own class with its own function to process an input and return success or failure
bool URGX_InputHandlerComponent::ProcessInputToken(FRGX_InputToken& InputToken)
{
	bool bResult = true;

	if (InputToken.ProcessMode == ERGX_InputProcessMode::Hold)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input %d Process hold\n"), (uint16)InputToken.InputID);

		if (InputToInfoMap[(uint16)InputToken.InputID].HoldTime < InputToken.HoldTime)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Not Enough Hold Time\n"));
			bResult = false;
		}

		if (InputToInfoMap[(uint16)InputToken.InputID].bReleased != InputToken.bRelease)
		{
			bResult = false;
		}
	}

	return bResult;
}

bool URGX_InputHandlerComponent::IsInputPressed(const ERGX_PlayerInputID InputID)
{
	return ((uint16)InputID & InputPressedState) > 0;
}

bool URGX_InputHandlerComponent::IsInputPressed(const uint16 InputID)
{
	return (InputID & InputPressedState) > 0;
}

void URGX_InputHandlerComponent::ConsumeInputChain(FRGX_InputChainInfo& InputChain)
{
	for (FRGX_InputToken& InputToken : InputChain.Inputs)
	{
		ConsumeInput((uint16)InputToken.InputID);
	}
}

void URGX_InputHandlerComponent::ConsumeInput(const uint16 InputID)
{
	InputToInfoMap[InputID].bConsumed = true;
}
