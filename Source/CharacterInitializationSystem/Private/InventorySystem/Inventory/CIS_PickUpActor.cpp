// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory/CIS_PickUpActor.h"
#include "AbilitySystem/CIS_AbilitySystemComponent.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_PG_Inventory_Message_ObjectOpen, "CIS.Inventory.Object.State.Open");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_PG_Inventory_Message_UpdateItems, "CIS.Inventory.Object.State.UpdateItems");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_PG_Inventory_Message_ObjectEmpty, "CIS.Inventory.Object.State.Empty");

// Sets default values
ACIS_PickUpActor::ACIS_PickUpActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}
