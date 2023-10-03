// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PG_MainMenu_PlayerController.h"

void APG_MainMenu_PlayerController::OnPossess(APawn* aPawn)
{
	if(aPawn)
		Super::OnPossess(aPawn);
}

void APG_MainMenu_PlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	APlayerController::PostProcessInput(DeltaTime, bGamePaused);
}
