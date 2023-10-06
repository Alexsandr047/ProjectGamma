// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PG_PlayerController.h"
#include "Core/PG_AssetManager.h"
#include "GameModes/PG_ProceduralWorld_GameMode.h"
#include "Kismet/GameplayStatics.h"

APG_PlayerController::APG_PlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void APG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void APG_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
}
