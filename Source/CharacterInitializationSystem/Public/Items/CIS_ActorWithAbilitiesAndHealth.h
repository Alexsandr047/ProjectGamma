// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CIS_HealthComponent.h"
#include "Items/CIS_ActorWithAbilities.h"
#include "CIS_ActorWithAbilitiesAndHealth.generated.h"


class UCIS_HealthSet;
class UCIS_CombatSet;
/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_ActorWithAbilitiesAndHealth : public ACIS_ActorWithAbilities
{
	GENERATED_BODY()


public:

	ACIS_ActorWithAbilitiesAndHealth(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

protected:
	//~AXCLActorWithAbilities implementation
	virtual void InitializeAbilitySystem() override;
	virtual void UninitializeAbilitySystem() override;
	//~End of AXCLActorWithAbilities implementation

protected:
	/**
	 * CIS "Combat Set" is required by Lyra Healing & Damage Effect Calculations.
	 *
	 * This set defines base healing or damage values.
	 *
	 * It should probably be optional (it defaults to all zeros),
	 * but if it does not exist, Lyra will often complain via warning log messages.
	 * 
	 * By default we just use all zeros for these attributes.
	 *
	 * Override as needed.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="XCL|Ability")
	TObjectPtr<UCIS_CombatSet> CombatSet;

	/**
	 * CIS "Health Set" is required by Lyra Healing & Damage Effect Calculations.
	 *
	 * This defines health, max health, incoming healing and incoming damage.
	 *
	 * You probably want to derive your own class from this to add your custom
	 * attributes to this one.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="XCL|Ability")
	TObjectPtr<UCIS_HealthSet> HealthSet;

	/**
	 * CIS "Health Component" is a convenience interface for HealthSet
	 *
	 * This requires the HealthSet and plugs into its event handlers.
	 * This makes it easier to respond to these events in BPs, for example.
	 *
	 * This component also seems to be what is responsible for handling the
	 * death of an Actor via network replication.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="XCL|Ability")
	TObjectPtr<UCIS_HealthComponent> HealthComponent;
};
