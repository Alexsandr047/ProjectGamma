// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_EquipmentItem.h"
#include "InventorySystem/Equipment/CIS_EquipmentDefinition.h"
#include "GameplayTagContainer.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_AttachmentItem.h"


void UCIS_ItemFragment_EquipmentItem::SetAttachmentDefinition(TArray<UCIS_ItemDefinition*> ItemDefinitions)
{
	AttachmentDefinition = ItemDefinitions;
}

TArray<FGameplayTag> UCIS_ItemFragment_EquipmentItem::InitAttachableFragments(UObject* Outer, UCIS_ItemInstance* Instance)
{
	UCIS_AttachmentLogic* Logic = Cast<UCIS_AttachmentLogic>(AttachmentLogic.Get()->ClassDefaultObject);
	TArray<UCIS_ItemDefinition*> TempAttachmentDefinitions;
	TempAttachmentDefinitions.Empty();
	TempAttachmentDefinitions = AttachmentDefinition;
	AttachmentDefinition.Empty();
	TArray<FGameplayTag> SlotsTags;
	SlotsTags.Empty();
	SlotsTags = Logic->GetAttachmentSlots(TempAttachmentDefinitions);

	for(auto& Definition: TempAttachmentDefinitions)
	{		
		if(Logic->CheckForUseOfAttachment(AttachmentDefinition, Definition))
		{
			for (UCIS_ItemFragment* Fragment : Definition->Fragments)
			{
				if (Fragment != nullptr)
				{
					Fragment->OnInstanceCreated(Instance);
				}
			}
			AttachmentDefinition.Add(Definition);
		}
	}		

	return SlotsTags;
}

TArray<TSubclassOf<ACIS_AttachmentItem>> UCIS_ItemFragment_EquipmentItem::GetAttachmentItem() const
{
	TArray<TSubclassOf<ACIS_AttachmentItem>> AttachmentItems;
	AttachmentItems.Empty();
	for(auto& Definition :AttachmentDefinition)
	{
		if(const UCIS_ItemFragment_AttachmentItem* Attachable = Cast<UCIS_ItemFragment_AttachmentItem>(Definition->FindFragmentByClass(UCIS_ItemFragment_AttachmentItem::StaticClass())))
		{
			AttachmentItems.Add(Attachable->ActorToSpawn);
		}
	}
	return AttachmentItems;
}

TArray<TObjectPtr<const UCIS_AbilitySet>> UCIS_ItemFragment_EquipmentItem::GetAbilitySets() const
{
	TArray<TObjectPtr<const UCIS_AbilitySet>> AbilitySets;
	AbilitySets.Empty();

	TArray<FGameplayTag> DefaultAbilityKeys;
	DefaultAbilityKeys.Empty();
	DefaultAbility.GetKeys(DefaultAbilityKeys);

	TArray<FGameplayTagContainer> AttachmentAbilityKeys;
	AttachmentAbilityKeys.Empty();
	
	for(auto& Definition :AttachmentDefinition)
	{
		if(const UCIS_ItemFragment_AttachmentItem* Attachable = Cast<UCIS_ItemFragment_AttachmentItem>(Definition->FindFragmentByClass(UCIS_ItemFragment_AttachmentItem::StaticClass())))
		{
			AbilitySets.Add(Attachable->AbilitySetsToGrant);
			AttachmentAbilityKeys.Add(Definition->GetItemTags());
		}
	}

	for(auto& DefaultKey: DefaultAbilityKeys)
	{
		bool bFind = false;
	
		for(auto& AttachmentKey: AttachmentAbilityKeys)
		{
			if(AttachmentKey.HasTag(DefaultKey))
			{
				bFind =  true;
				break;
			}			
		}
		if(!bFind)
		{
			if (const auto* Ability = DefaultAbility.Find(DefaultKey))
			{
				AbilitySets.Add(*Ability);
			}
		}
	}

	return AbilitySets;
}
