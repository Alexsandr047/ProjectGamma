// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CIS_AbilitySystemGlobals.h"

#include "AbilitySystem/CIS_GameplayEffectContext.h"

UCIS_AbilitySystemGlobals::UCIS_AbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}



FGameplayEffectContext* UCIS_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FCIS_GameplayEffectContext();
}
