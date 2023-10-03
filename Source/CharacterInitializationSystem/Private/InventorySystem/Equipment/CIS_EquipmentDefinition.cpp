// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Equipment/CIS_EquipmentDefinition.h"
#include "InventorySystem/Equipment/CIS_EquipmentInstance.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"

UCIS_AttachmentLogic::UCIS_AttachmentLogic(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

UCIS_EquipmentDefinition::UCIS_EquipmentDefinition(const FObjectInitializer& ObjectInitializer)
{
	InstanceType = UCIS_EquipmentInstance::StaticClass();
}

void UCIS_EquipmentDefinition::SetParams(TSubclassOf<UCIS_EquipmentInstance> _InstanceType, TArray<TObjectPtr<const UCIS_AbilitySet>> _AbilitySetsToGrant,
TSubclassOf<ACIS_EquipmentItem> _ActorToSpawn, TArray<TSubclassOf<ACIS_AttachmentItem>> _AttachmentItems)
{
	InstanceType = _InstanceType;
	AbilitySetsToGrant = _AbilitySetsToGrant;
	ActorToSpawn = _ActorToSpawn;
	AttachmentItems = _AttachmentItems;
}
