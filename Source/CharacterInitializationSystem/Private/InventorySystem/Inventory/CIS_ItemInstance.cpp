// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory/CIS_ItemInstance.h"

#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_AttachmentItem.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_DescriptionItem.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_EquipmentItem.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_SetStats.h"
#include "Net/UnrealNetwork.h"

UCIS_ItemInstance::UCIS_ItemInstance(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}
void UCIS_ItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, AttachableParam);
	
	UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass) BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
}
UWorld* UCIS_ItemInstance::GetWorld() const
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

APawn* UCIS_ItemInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void UCIS_ItemInstance::SetItemDef(UCIS_ItemDefinition* InItemDef)
{
	ItemDef = InItemDef;	
}

void UCIS_ItemInstance::InitAttachable(TArray<UCIS_ItemFragment*> ItemFragments, UCIS_ItemInstance* Instance)
{
	TArray<FAttachableParam> Data;
	Data.Empty();
	TArray<FGameplayTag> SlotsTags;
	SlotsTags.Empty();

	for(auto& Fragment : ItemFragments)
	{
		if(UCIS_ItemFragment_EquipmentItem* EquipmentFragment= Cast<UCIS_ItemFragment_EquipmentItem>(Fragment))
		{		
			SlotsTags = EquipmentFragment->InitAttachableFragments(this, Instance);
			for(auto& SlotTag: SlotsTags)
			{
				FAttachableParam ItemParam;
				ItemParam.ItemSlotTag = SlotTag;
				for(auto& AttachDef: EquipmentFragment->AttachmentDefinition)
				{
					if(AttachDef->ItemTags.HasTag(SlotTag))
					{
						if(UCIS_ItemFragment_DescriptionItem* ItemDescription = Cast<UCIS_ItemFragment_DescriptionItem>(AttachDef->FindFragmentByClass(UCIS_ItemFragment_DescriptionItem::StaticClass())))
							ItemParam.ItemDescriptionId = ItemDescription->DescriptionItemID;
						break;
					}
				}
				Data.Add(ItemParam);
			}
		}
	}
	AttachableParam = Data;
}

void UCIS_ItemInstance::SetUpAttachmentFragment(UCIS_ItemDefinition* AttachmentDef)
{
	if(!AttachmentDef)
		return;
	//Fragments.Empty();
	/*for(auto Fragment :ItemDef->Fragments)
	{
		if(UCIS_ItemFragment_EquipmentItem* EquipmentFragment =  Cast<UCIS_ItemFragment_EquipmentItem>(ItemDef->FindFragmentByClass(UCIS_ItemFragment_EquipmentItem::StaticClass())))
		{
			EquipmentFragment->AttachmentDefinition.Add(AttachmentDef);
			Fragments.Add()
		}
	}*/
}

void UCIS_ItemInstance::OnRep_ItemDefinition()
{
	
}

void UCIS_ItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UCIS_ItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UCIS_ItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UCIS_ItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

const UCIS_ItemFragment* UCIS_ItemInstance::FindFragmentByClass(TSubclassOf<UCIS_ItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return ItemDef->FindFragmentByClass(FragmentClass);
	}	
	return nullptr;
}

/*const UCIS_ItemFragment* UCIS_ItemInstance::FindDescriptionFragment(UCIS_ItemDefinition* ItemDefinition)
{
	
}*/
