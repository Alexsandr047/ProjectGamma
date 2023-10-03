// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_ItemManagerWindow.h"

#include "Core/CIS_CoreTypes.h"
#include "InventorySystem/PG_QuickBarComponent.h"
#include "InventorySystem/Inventory/CIS_ItemInstance.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_DescriptionItem.h"


void UPG_ItemManagerWindow::MakeItems(UPG_QuickBarComponent* QuickBarComponent, EPG_EWeaponSlots EquipmentsSlots, UCIS_ItemInstance*& WeaponSlot)
{
	if(!QuickBarComponent)
		return;

	UCIS_ItemInstance* ItemSlot;
	QuickBarComponent->GetWeaponSlots(EquipmentsSlots ,ItemSlot);
	if(ItemSlot)
		WeaponSlot = ItemSlot;
			

}

FString UPG_ItemManagerWindow::GetItemDescription(UCIS_ItemInstance* ItemSlot)
{
	if(ItemSlot)
	{
		if(UCIS_ItemFragment_DescriptionItem* DescriptionItem =  Cast<UCIS_ItemFragment_DescriptionItem>(ItemSlot->GetItemDef()->FindFragmentByClass(UCIS_ItemFragment_DescriptionItem::StaticClass())))
		{
			return DescriptionItem->DescriptionItemID;
		}
	}
	return  "";
}


bool  UPG_ItemManagerWindow::MakeAttachmentsSlots(UCIS_ItemInstance* ItemSlot, TMap<FGameplayTag, FString>& SlotTags)
{
	if(!ItemSlot)
		return false;

	TMap<FGameplayTag, FString> Slots;
	for(auto& SlotTag: ItemSlot->GetAttachableParam())
	{
		Slots.Add(SlotTag.ItemSlotTag, SlotTag.ItemDescriptionId);
	}
	SlotTags = Slots;
	return true;
}

