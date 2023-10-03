// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/PG_QuickBarComponent.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"
#include "PG_ItemManagerWindow.generated.h"

class UPG_QuickBarComponent;
class UCIS_ItemInstance;
class UCIS_AttachmentLogic;

UCLASS()
class PROJECTGAMMA_API UPG_ItemManagerWindow : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void MakeItems(UPG_QuickBarComponent* QuickBarComponent, EPG_EWeaponSlots EquipmentsSlots, UCIS_ItemInstance*& WeaponSlot);

	UFUNCTION(BlueprintCallable)
	FString GetItemDescription(UCIS_ItemInstance* ItemSlot);
	UFUNCTION(BlueprintCallable)
	bool MakeAttachmentsSlots(UCIS_ItemInstance* ItemSlot, TMap<FGameplayTag, FString>& SlotTags);
	UPROPERTY()
	TSubclassOf<UCIS_AttachmentLogic> CurrentAttachmentLogic;
	UPROPERTY()
	UCIS_AttachmentLogic* AttachmentLogic;
	
};

