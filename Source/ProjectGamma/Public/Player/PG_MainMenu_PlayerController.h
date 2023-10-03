// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/PG_PlayerController.h"
#include "PG_MainMenu_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API APG_MainMenu_PlayerController : public APG_PlayerController
{
	GENERATED_BODY()

public:

	virtual void OnPossess(APawn* aPawn) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
