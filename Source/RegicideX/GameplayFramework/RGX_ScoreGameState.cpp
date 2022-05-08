// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ScoreGameState.h"

ARGX_ScoreGameState::ARGX_ScoreGameState()
{
	Score = 0;
	Round = 1;
	NumEnemies = 0;
	
}

bool ARGX_ScoreGameState::EnemyDeath(const int Type)
{
	switch (Type)
	{
	case 0:
		Score += 50;
		break;
	case 1:
		Score += 50;
		break;
	case 2:
		Score += 50;
		break;
	case 3:
		Score += 50;
		break;
	default:
		Score += 5;
	}

	NumEnemies--;
	
	return NumEnemies == 0;
}

int ARGX_ScoreGameState::NextRound()
{
	Score += 100 * Round;
	return Round++;
}

void ARGX_ScoreGameState::SetNumEnemies(const int NewNumEnemies)
{
	NumEnemies = NewNumEnemies;
}

