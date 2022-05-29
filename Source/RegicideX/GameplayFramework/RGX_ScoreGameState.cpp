// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_ScoreGameState.h"

int ARGX_ScoreGameState::GetNumEnemies() const
{
	return NumEnemies;
}

int ARGX_ScoreGameState::GetScore() const
{
	return Score;
}

int ARGX_ScoreGameState::GetRound() const
{
	return Round;
}

void ARGX_ScoreGameState::SetScore(const int NewScore)
{
	this->Score = NewScore;
}

ARGX_ScoreGameState::ARGX_ScoreGameState()
{
	Score = 0;
	Round = 1;
	NumEnemies = 0;
}

void ARGX_ScoreGameState::SetStateDefaults()
{
	Score = 0;
	Round = 1;
	NumEnemies = 0;
}

bool ARGX_ScoreGameState::OnEnemyDeath(const int Type)
{
	// @todo: Score system based on DataTable / DataAsset System
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

int ARGX_ScoreGameState::StartNextRound()
{
	// @todo: Score system based on DataTable / DataAsset System
	Score += 100 * Round;
	return Round++;
}

void ARGX_ScoreGameState::SetNumEnemies(const int NewNumEnemies)
{
	NumEnemies = NewNumEnemies;
}

void ARGX_ScoreGameState::IncreaseScore()
{
	// @todo: Score system based on DataTable / DataAsset System
}