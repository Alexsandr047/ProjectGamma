
#include "AbilitySystem/AttributeSet/CIS_CombatSet.h"

#include "Net/UnrealNetwork.h"

UCIS_CombatSet::UCIS_CombatSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
{
}

void UCIS_CombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCIS_CombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCIS_CombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UCIS_CombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCIS_CombatSet, BaseDamage, OldValue);
}

void UCIS_CombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCIS_CombatSet, BaseHeal, OldValue);
}
