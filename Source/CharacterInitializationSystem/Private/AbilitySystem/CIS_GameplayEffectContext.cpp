
#include "AbilitySystem/CIS_GameplayEffectContext.h"
#include "AbilitySystem/CIS_AbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


/*#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayEffectContextHandle)*/


FCIS_GameplayEffectContext* FCIS_GameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FCIS_GameplayEffectContext::StaticStruct()))
	{
		return (FCIS_GameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FCIS_GameplayEffectContext::SetAbilitySource(const ICIS_AbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const ICIS_AbilitySourceInterface* FCIS_GameplayEffectContext::GetAbilitySource() const
{
	return Cast<ICIS_AbilitySourceInterface>(AbilitySourceObject.Get());
}

bool FCIS_GameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

const UPhysicalMaterial* FCIS_GameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}
