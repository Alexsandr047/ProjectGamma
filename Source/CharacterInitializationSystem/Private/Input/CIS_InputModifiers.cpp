// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/CIS_InputModifiers.h"
#include "EnhancedPlayerInput.h"

namespace CIS_InputModifiersHelpers
{
	/** Returns the owning CISLocalPlayer of an Enhanced Player Input pointer */
	static ULocalPlayer* GetLocalPlayer(const UEnhancedPlayerInput* PlayerInput)
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


FInputActionValue UCIS_SettingBasedScalar::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
	FInputActionValue CurrentValue, float DeltaTime)
{
	if (ensureMsgf(CurrentValue.GetValueType() != EInputActionValueType::Boolean, TEXT("Setting Based Scalar modifier doesn't support boolean values.")))
	{
		if (ULocalPlayer* LocalPlayer = CIS_InputModifiersHelpers::GetLocalPlayer(PlayerInput))
		{

			FVector ScalarToUse = FVector(1.0, 1.0, 1.0);
			
			ScalarToUse.X = FMath::Clamp(ScalarToUse.X, MinValueClamp.X, MaxValueClamp.X);
			ScalarToUse.Y = FMath::Clamp(ScalarToUse.Y, MinValueClamp.Y, MaxValueClamp.Y);
			ScalarToUse.Z = FMath::Clamp(ScalarToUse.Z, MinValueClamp.Z, MaxValueClamp.Z);
			
			return CurrentValue.Get<FVector>() * ScalarToUse;
		}
	}
	return CurrentValue;	
}
