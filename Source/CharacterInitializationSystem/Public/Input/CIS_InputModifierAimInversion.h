// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "CIS_InputModifierAimInversion.generated.h"

/**
 * 
 */
/** Applies an inversion of axis values based on a setting in the Liberty Shared game settings */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_InputModifierAimInversion : public UInputModifier
{
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
	
};
