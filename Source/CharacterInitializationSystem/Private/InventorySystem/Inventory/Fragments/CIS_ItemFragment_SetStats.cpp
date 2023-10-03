// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_SetStats.h"

#include "InventorySystem/Inventory/CIS_ItemInstance.h"

void UCIS_ItemFragment_SetStats::OnInstanceCreated(UCIS_ItemInstance* Instance) const
{
	for (const auto& KVP : InitialItemStats)
	{
		Instance->AddStatTagStack(KVP.Key, KVP.Value);
	}
}

void UCIS_ItemFragment_SetStats::SetTagStats(FGameplayTag Tag, int32 Stat)
{
	if(!InitialItemStats.Find(Tag))
		InitialItemStats.Add(Tag,Stat);
}

int32 UCIS_ItemFragment_SetStats::GetItemStatByTag(FGameplayTag Tag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(Tag))
	{
		return *StatPtr;
	}

	return 0;	
}

TMap<FGameplayTag, int32> UCIS_ItemFragment_SetStats::GetItemStats() const
{
	return InitialItemStats;
}
