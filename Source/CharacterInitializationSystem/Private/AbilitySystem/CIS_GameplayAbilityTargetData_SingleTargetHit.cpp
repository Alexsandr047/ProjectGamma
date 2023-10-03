// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CIS_GameplayAbilityTargetData_SingleTargetHit.h"

#include "AbilitySystem/CIS_GameplayEffectContext.h"

struct FGameplayEffectContextHandle;

//////////////////////////////////////////////////////////////////////

void FCIS_GameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);

	// Add game-specific data
	if (FCIS_GameplayEffectContext* TypedContext = FCIS_GameplayEffectContext::ExtractEffectContext(Context))
	{
		TypedContext->CartridgeID = CartridgeID;
	}
}

bool FCIS_GameplayAbilityTargetData_SingleTargetHit::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);

	Ar << CartridgeID;

	return true;
}

