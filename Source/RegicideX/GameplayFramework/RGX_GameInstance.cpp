// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GameplayFramework/RGX_GameInstance.h"
//#include "MoviePlayer.h"

void URGX_GameInstance::BeginLoadingScreen(const FString& MapName)
{
	if(!IsRunningDedicatedServer())
	{
		//FLoadingScreenAttributes LoadingScreen;
	}
}

void URGX_GameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{

}
