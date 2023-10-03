// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/CIS_InputModifierAimInversion.h"

#include "EnhancedPlayerInput.h"

namespace CIS_InputModifiersHelpers
{
	/** Returns the owning LyraLocalPlayer of an Enhanced Player Input pointer */
	static ULocalPlayer* GetLocalPlayer2(const UEnhancedPlayerInput* PlayerInput)
	{
		if (PlayerInput)
		{
			if (APlayerController* PC = Cast<APlayerController>(PlayerInput->GetOuter()))
			{
				return Cast<ULocalPlayer>(PC->GetLocalPlayer());
			}
		}
		return nullptr;
	}
	
}

FInputActionValue UCIS_InputModifierAimInversion::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
	FInputActionValue CurrentValue, float DeltaTime)
{
	ULocalPlayer* LocalPlayer = CIS_InputModifiersHelpers::GetLocalPlayer2(PlayerInput);
	if (!LocalPlayer)
	{
		return CurrentValue;
	}

	//TODO Нужно сделать логику для настроек меню
	FVector NewValue = CurrentValue.Get<FVector>();; 
	NewValue.Y *= -1.0f;
	NewValue.X *= 1.0f;
	
	return NewValue;
}

