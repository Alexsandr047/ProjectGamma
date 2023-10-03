// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/CIS_AttributeSet.h"

#include "AbilitySystem/CIS_AbilitySystemComponent.h"

UCIS_AttributeSet::UCIS_AttributeSet()
{
}

UWorld* UCIS_AttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UCIS_AbilitySystemComponent* UCIS_AttributeSet::GetLyraAbilitySystemComponent() const
{
	return Cast<UCIS_AbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
