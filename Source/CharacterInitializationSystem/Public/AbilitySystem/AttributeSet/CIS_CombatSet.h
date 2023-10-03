#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CIS_AttributeSet.h"
#include "CIS_CombatSet.generated.h"


/**
 * UCIS_CombatSet
 *
 *  Class that defines attributes that are necessary for applying damage or healing.
 *	Attribute examples include: damage, healing, attack power, and shield penetrations.
 */
UCLASS(BlueprintType)
class UCIS_CombatSet : public UCIS_AttributeSet
{
	GENERATED_BODY()

public:

	UCIS_CombatSet();

	ATTRIBUTE_ACCESSORS(UCIS_CombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(UCIS_CombatSet, BaseHeal);

protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

private:

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "CIS|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "CIS|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
};
