// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "InventorySystem/PG_QuickBarComponent.h"
#include "InventorySystem/Inventory/CIS_ItemInstance.h"
#include "PG_InventoryWindow.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTGAMMA_API UPG_InventoryWindow : public UUserWidget
{
	GENERATED_BODY()

public:
		
	UFUNCTION(BlueprintCallable)
	void GetItem(APlayerController* PlayerController, const FGameplayTagContainer& ItemTags);

	UFUNCTION(BlueprintCallable)
	void GetAllItem(FPG_GetItemsInInventoryMessage ItemsInInventoryMessage);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateItemWidget(const FString& DescriptionItemId, int32 InventorySlot);

	
};
