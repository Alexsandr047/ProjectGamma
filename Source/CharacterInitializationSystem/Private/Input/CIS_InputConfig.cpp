// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/CIS_InputConfig.h"

/*UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Look_Mouse, "InputTag.Look.Mouse");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Move, "InputTag.Move");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Crouch, "InputTag.Crouch");*/

UCIS_InputConfig::UCIS_InputConfig(const FObjectInitializer& ObjectInitializer)
{
	
}

const UInputAction* UCIS_InputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FCIS_InputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UCIS_InputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FCIS_InputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
