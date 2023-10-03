// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PG_ShipBasePlayerController.h"

APG_ShipBasePlayerController::APG_ShipBasePlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void APG_ShipBasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
}
