// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Equipment/CIS_QuickBarComponent.h"
#include "Core/CIS_CoreTypes.h"
#include "InventorySystem/Equipment/CIS_EquipmentDefinition.h"
#include "InventorySystem/Equipment/CIS_EquipmentInstance.h"
#include "InventorySystem/Equipment/CIS_EquipmentManagerComponent.h"
#include "InventorySystem/Inventory/CIS_ItemInstance.h"
#include "InventorySystem/Inventory/CIS_ItemManagerComponent.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_AttachmentItem.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_EquipmentItem.h"
#include "Net/UnrealNetwork.h"

UCIS_QuickBarComponent::UCIS_QuickBarComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);


	
}

void UCIS_QuickBarComponent::BeginPlay()
{
	
	if (InventorySlots.Num() < NumBaseInventorySlot)
	{
		InventorySlots.AddDefaulted(NumBaseInventorySlot - InventorySlots.Num());
	}
	
	Super::BeginPlay();
}

UCIS_ItemManagerComponent* UCIS_QuickBarComponent::FindItemManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{		
		return OwnerController->FindComponentByClass<UCIS_ItemManagerComponent>();
	}
	return nullptr;
}

UCIS_EquipmentManagerComponent* UCIS_QuickBarComponent::FindEquipmentManager() const
{
	if(AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if(APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UCIS_EquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UCIS_QuickBarComponent::OnRep_InventorySlots()
{
	UpdateInventory.Broadcast();
}

bool UCIS_QuickBarComponent::ClearInventorySlot(int32 Slot)
{
	if(Slot != -1 || Slot < InventorySlots.Num())
	{
		InventorySlots[Slot] = nullptr;
		OnRep_InventorySlots();
		return true;
	}
	return false;
}

TArray<UCIS_ItemInstance*> UCIS_QuickBarComponent::GetInventorySlots() const
{
	return InventorySlots;
}

UCIS_ItemInstance* UCIS_QuickBarComponent::GetItemInInventorySlot(int32 Slot) const
{
	if(Slot < 0)
		return nullptr;

	UCIS_ItemInstance* Item = nullptr;
	if(Slot < InventorySlots.Num())
		Item = InventorySlots[Slot];
	
	return Item;
}

bool UCIS_QuickBarComponent::AddItemInInventorySlot(int32 Slot, UCIS_ItemInstance* Item)
{
	if(Slot < 0)
		return false;
	
	if(Item && Slot < InventorySlots.Num())
	{
		InventorySlots[Slot] = Item;
		OnRep_InventorySlots();
		return true;
	}
	return false;
}

bool UCIS_QuickBarComponent::RemoveItemInInventorySlot(int32 Slot)
{
	if(Slot < 0)
		return false;

	if(Slot < InventorySlots.Num())
	{
		InventorySlots[Slot] = nullptr;
		OnRep_InventorySlots();
		return true;
	}
	return false;
}

UCIS_ItemInstance* UCIS_QuickBarComponent::TakeItemFromInventorySlot(int32 Slot)
{
	if(Slot < 0)
		return false;
	
	if(Slot < InventorySlots.Num())
	{
		UCIS_ItemInstance* Item = InventorySlots[Slot-1];
		InventorySlots[Slot-1] = nullptr;
		return Item;
	}
	return nullptr;
}

void UCIS_QuickBarComponent::SwapItemsInInventory(int32 SlotOne, int32 SlotTwo)
{
	if( SlotOne == -1 || SlotTwo == -1 || SlotOne >= InventorySlots.Num() || SlotTwo >= InventorySlots.Num() || SlotOne == SlotTwo)
		return;

	UCIS_ItemInstance* ItemOne = InventorySlots[SlotOne];
	UCIS_ItemInstance* ItemTwo = InventorySlots[SlotTwo];

	if (InventorySlots[SlotOne])
		InventorySlots[SlotOne] = nullptr;	

	if(InventorySlots[SlotTwo])
		InventorySlots[SlotTwo] = nullptr;

	if (ItemOne)
		InventorySlots[SlotTwo] = ItemOne;

	if(ItemTwo)
		InventorySlots[SlotOne] = ItemTwo;
	
	MovingAnItemInInventory.Broadcast();
	bMovingAnItemInInventory = !bMovingAnItemInInventory;
}

void UCIS_QuickBarComponent::OnRep_MovingAnItemInInventory()
{
	MovingAnItemInInventory.Broadcast();
}


void UCIS_QuickBarComponent::LoadItemsToInventory(UCIS_ItemManagerComponent* ItemManager,const TArray<UCIS_ItemDefinition*>& Items)
{
	
	for(int32 i = 0; i < Items.Num(); ++i)
	{
		bool bItemAdd = false;
		for(int32 j = 0; j < InventorySlots.Num(); ++j)
		{
			if(!bItemAdd && InventorySlots.Num() >0 && i < InventorySlots.Num() && !InventorySlots[j])
			{
				UCIS_ItemInstance* Item = ItemManager->AddItemDefinition(Items[i], 1);
				AddItemInInventorySlot(j, Item);
				bItemAdd = true;
			}
		}
	}
	//GEngine->ForceGarbageCollection();
}

void UCIS_QuickBarComponent::EquipItemInSlot(UCIS_ItemInstance*& Item, UCIS_EquipmentInstance*& Target,
	UCIS_ItemInstance*& EquipSlotItem, FName SocketName)
{
	if(EquipSlotItem)
		return;
	
	if (Item != nullptr)
	{
		EquipSlotItem = Item;
		check(Target == nullptr);
		if(UCIS_ItemInstance* SlotItem = EquipSlotItem)
		{
			if(const UCIS_ItemFragment_EquipmentItem* EquipInfo = SlotItem->FindFragmentByClass<UCIS_ItemFragment_EquipmentItem>())
			{
				UCIS_EquipmentDefinition* EquipDef = NewObject<UCIS_EquipmentDefinition>(this);

				EquipDef->SetParams(EquipInfo->InstanceType, EquipInfo->GetAbilitySets(), EquipInfo->ActorToSpawn, EquipInfo->GetAttachmentItem());
				if(EquipDef != nullptr)
				{
					if (UCIS_EquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
					{			
						Target = Cast<UCIS_EquipmentInstance>(EquipmentManager->EquipItem(EquipDef, SocketName));
						if (Target != nullptr)
						{
							Target->SetInstigator(SlotItem);							
						}
					}
				}				
			}
		}
	}
}

bool UCIS_QuickBarComponent::ClearItemInSlot(UCIS_ItemInstance*& EquipSlotItem, UCIS_EquipmentInstance*& Item)
{
	if(EquipSlotItem)
	{
		if(UCIS_EquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
		{	
			EquipmentManager->UnequipItem(Item);
			Item = nullptr;
			EquipSlotItem = nullptr;
			return true;		
		}
	}
	return false;
}

void UCIS_QuickBarComponent::SetUpAttachmentOnItem(UCIS_ItemInstance*& EquipSlot,
                                                   UCIS_ItemDefinition*& AttachmentDefinition)
{
	if(EquipSlot && EquipSlot->GetItemDef())
	{
		if(UCIS_ItemFragment_AttachmentItem* Attachment = Cast<UCIS_ItemFragment_AttachmentItem>(AttachmentDefinition->FindFragmentByClass(UCIS_ItemFragment_AttachmentItem::StaticClass())))
		{
			EquipSlot->SetUpAttachmentFragment(AttachmentDefinition);
		}
	}
}

UCIS_ItemDefinition* UCIS_QuickBarComponent::GetAndRemoveAttachmentFromSlot(UCIS_ItemInstance*& EquipSlot/*, UCIS_ItemCategory* Categor*/)
{
	/*UCIS_ItemDefinition* AttachmentDef = nullptr;
	if(EquipSlot && Category)
	{
		if(UCIS_ItemFragment_EquipmentItem* ItemFragment = Cast<UCIS_ItemFragment_EquipmentItem>(EquipSlot->GetItemDef()->FindFragmentByClass(UCIS_ItemFragment_EquipmentItem::StaticClass())))
		{
			//ItemFragment
		}
	}*/
	return nullptr;
}


void UCIS_QuickBarComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventorySlots);
}
