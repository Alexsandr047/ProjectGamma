// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/PG_GameMode.h"
#include "PG_MainMenu_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API APG_MainMenu_GameMode : public APG_GameMode
{
	GENERATED_BODY()

public:

	APG_MainMenu_GameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
