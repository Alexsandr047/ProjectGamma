// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_InventoryWindow.h"
#include "InventorySystem/PG_QuickBarComponent.h"
#include "GameplayTagContainer.h"
#include "InventorySystem/Inventory/CIS_ItemInstance.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_DescriptionItem.h"


void UPG_InventoryWindow::GetItem(APlayerController* PlayerController, const FGameplayTagContainer& ItemTags)
{
	UPG_QuickBarComponent* QB = Cast<UPG_QuickBarComponent>(PlayerController->GetComponentByClass(UPG_QuickBarComponent::StaticClass()));
	if(QB)
	{
		for(int32 i = 0; i < QB->GetInventorySlots().Num(); ++ i)
		{
			if(auto Item = QB->GetInventorySlots()[i])
			{
				
				if(ItemTags.HasAny(Item->GetItemDef()->ItemTags))
				{
					if(UCIS_ItemFragment_DescriptionItem* Fragment = Cast<UCIS_ItemFragment_DescriptionItem>(Item->GetItemDef()->FindFragmentByClass(UCIS_ItemFragment_DescriptionItem::StaticClass())))
					{
						CreateItemWidget(Fragment->DescriptionItemID, i);
					}					
				}
			}
		}
	}
}

void UPG_InventoryWindow::GetAllItem(FPG_GetItemsInInventoryMessage ItemsInInventoryMessage)
{
	for(int32 i = 0; i < ItemsInInventoryMessage.Items.Num(); ++ i)
	{
		if(auto Item = ItemsInInventoryMessage.Items[i])
		{
			if(UCIS_ItemFragment_DescriptionItem* Fragment = Cast<UCIS_ItemFragment_DescriptionItem>(Item->GetItemDef()->FindFragmentByClass(UCIS_ItemFragment_DescriptionItem::StaticClass())))
			{
					CreateItemWidget(Fragment->DescriptionItemID, i);
			}					
		}
	}
}
