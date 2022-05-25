// Fill out your copyright notice in the Description page of Project Settings.


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
		uint16 BitMask = ~(uint16)NewInputID;
		InputPressedState = InputPressedState & BitMask;
		InputToInfoMap[(uint16)NewInputID].bPressedInAir = false;
	}
}

void URGX_InputHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

	for (TPair<ERGX_PlayerActions, FRGX_InputChainInfo>& InputToAction : InputToActionMap)
	{
		bool bResult = true;

		for (FRGX_InputToken InputToken : InputToAction.Value.Inputs)
		{
			if (InputToInfoMap[(uint16)InputToken.InputID].bPressedInAir != InputToken.bPressedInAir)
			{
				return;
			}

			if (ProcessInput(InputToken, DeltaTime) == false)
			{
				bResult = false;
			}
		}

		if (bResult == true)
		{
			ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(GetOwner());
			PlayerCharacter->HandleAction(InputToAction.Key);

			if (InputToAction.Value.bConsumeInput == true)
			{
				ConsumeInputChain(InputToAction.Value);
			}
		}
	}
}

bool URGX_InputHandlerComponent::ProcessInput(FRGX_InputToken& InputToken, float DeltaTime)
{
	bool bResult = true;

	UE_LOG(LogTemp, Warning, TEXT("Input: %d\n"), (uint16)InputToken.InputID);
	UE_LOG(LogTemp, Warning, TEXT("Input State: %d\n"), InputPressedState);

	if (InputToInfoMap[(uint16)InputToken.InputID].bConsumed == true)
		return false;

	if (IsInputPressed(InputToken.InputID))
	{
		UE_LOG(LogTemp, Warning, TEXT("Input %d Pressed\n"), (uint16)InputToken.InputID);
		if (ProcessInputToken(InputToken, DeltaTime) == false)
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
bool URGX_InputHandlerComponent::ProcessInputToken(FRGX_InputToken& InputToken, float DeltaTime)
{
	bool bResult = true;

	if (InputToken.ProcessMode == ERGX_InputProcessMode::Hold)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input %d Process hold\n"), (uint16)InputToken.InputID);

		if (InputToInfoMap[(uint16)InputToken.InputID].HoldTime < InputToken.HoldTime)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Enough Hold Time\n"));
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
