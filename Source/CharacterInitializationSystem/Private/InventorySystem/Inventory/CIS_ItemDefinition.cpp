// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory/CIS_ItemDefinition.h"

UWorld* UCIS_ItemFragment::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

APawn* UCIS_ItemFragment::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

UWorld* UCIS_ItemDefinition::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

APawn* UCIS_ItemDefinition::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

UCIS_ItemDefinition::UCIS_ItemDefinition(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

UCIS_ItemFragment* UCIS_ItemDefinition::FindFragmentByClass(TSubclassOf<UCIS_ItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UCIS_ItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

const UCIS_ItemFragment* UCISInventoryFunctionLibrary::FindItemDefinitionFragment(
	TSubclassOf<UCIS_ItemDefinition> ItemDef, TSubclassOf<UCIS_ItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UCIS_ItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
