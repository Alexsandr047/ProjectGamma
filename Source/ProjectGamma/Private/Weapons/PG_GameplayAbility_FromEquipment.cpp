// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/PG_GameplayAbility_FromEquipment.h"
#include "InventorySystem/Inventory/CIS_ItemInstance.h"

UPG_GameplayAbility_FromEquipment::UPG_GameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

UCIS_EquipmentInstance* UPG_GameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UCIS_EquipmentInstance>(Spec->SourceObject);
	}

	return nullptr;
}

UCIS_ItemInstance* UPG_GameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UCIS_EquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UCIS_ItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}

#if WITH_EDITOR
EDataValidationResult UPG_GameplayAbility_FromEquipment::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = Super::IsDataValid(ValidationErrors);
	
	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		ValidationErrors.Add(NSLOCTEXT("Lyra", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}
	
	return Result;
}

#endif