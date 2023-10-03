// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/PG_MainMenu_GameMode.h"

APG_MainMenu_GameMode::APG_MainMenu_GameMode()
{
	
}

void APG_MainMenu_GameMode::PostLogin(APlayerController* NewPlayer)
{
	AGameMode::PostLogin(NewPlayer);
}
