// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CIS_ActorWithAbilitiesAndHealth.h"
#include "AbilitySystem/CIS_AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/CIS_CombatSet.h"
#include "AbilitySystem/AttributeSet/CIS_HealthSet.h"

ACIS_ActorWithAbilitiesAndHealth::ACIS_ActorWithAbilitiesAndHealth(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CombatSet = ObjectInitializer.CreateDefaultSubobject<UCIS_CombatSet>(this, TEXT("Combat Set"));
	HealthSet = ObjectInitializer.CreateDefaultSubobject<UCIS_HealthSet>(this, TEXT("Health Set"));

	HealthComponent = ObjectInitializer.CreateDefaultSubobject<UCIS_HealthComponent>(this, TEXT("Health Component"));
}

void ACIS_ActorWithAbilitiesAndHealth::InitializeAbilitySystem()
{
	// Super::InitializeAbilitySystem is required to get the ASC working, do this first:
	Super::InitializeAbilitySystem();

	// HealthComponent requires a Lyra based ASC
	UCIS_AbilitySystemComponent* ASC = GetCISAbilitySystemComponentChecked();

	// Add Attribute Sets to ASC
	ASC->AddAttributeSetSubobject(CombatSet.Get());
	ASC->AddAttributeSetSubobject(HealthSet.Get());

	// DO NOT init HealthComponent until AFTER HealthSet has been added
	HealthComponent->InitializeWithAbilitySystem(ASC);
}

void ACIS_ActorWithAbilitiesAndHealth::UninitializeAbilitySystem()
{
	HealthComponent->UninitializeFromAbilitySystem();

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RemoveSpawnedAttribute(CombatSet.Get());
		ASC->RemoveSpawnedAttribute(HealthSet.Get());
	}
	
	Super::UninitializeAbilitySystem();
}
