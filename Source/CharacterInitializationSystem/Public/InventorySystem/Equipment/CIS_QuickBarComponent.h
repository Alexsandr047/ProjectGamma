// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "Engine/DataTable.h"
#include "CIS_QuickBarComponent.generated.h"

class UCIS_ItemManagerComponent;
class UCIS_EquipmentManagerComponent;
class UCIS_ItemInstance;
class UCIS_EquipmentInstance;
class UCIS_WeaponInstance;
class UCIS_ItemDefinition;


/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipUpdate);

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_QuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	UCIS_QuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_InventorySlots();
	
	UPROPERTY(EditAnywhere)
	int32 NumBaseInventorySlot = 30;
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Inventory|InventorySlots")
	UCIS_ItemInstance* GetItemInInventorySlot(int32 Slot = -1) const;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintPure=false, Category = "Inventory|InventorySlots")
	bool RemoveItemInInventorySlot(int32 Slot = -1);

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Inventory|InventorySlots")
	UCIS_ItemInstance* TakeItemFromInventorySlot(int32 Slot =-1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintPure=false)
	void SwapItemsInInventory(int32 SlotOne = -1, int32 SlotTwo = -1);
		
	UPROPERTY(BlueprintAssignable)
	FOnEquipUpdate MovingAnItemInInventory;

	UFUNCTION()
	void OnRep_MovingAnItemInInventory();
	
	UPROPERTY(ReplicatedUsing=OnRep_MovingAnItemInInventory)
	bool bMovingAnItemInInventory;
	
public:
	//AlL
	UCIS_ItemManagerComponent* FindItemManager() const;
	UCIS_EquipmentManagerComponent * FindEquipmentManager() const;

	UPROPERTY(BlueprintAssignable)
	FOnEquipUpdate UpdateInventory;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintPure=false, Category = "Inventory|InventorySlots")
	bool AddItemInInventorySlot(int32 Slot = -1, UCIS_ItemInstance* Item = nullptr);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintPure=false, Category = "Inventory|InventorySlots")
	bool ClearInventorySlot(int32 Slot = -1);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintPure=false)
	void LoadItemsToInventory(UCIS_ItemManagerComponent* ItemManager, const TArray<UCIS_ItemDefinition*>& Items);

	void EquipItemInSlot(UCIS_ItemInstance*& Item, UCIS_EquipmentInstance*& Target, UCIS_ItemInstance*& EquipSlotItem, FName SocketName);
	bool ClearItemInSlot(UCIS_ItemInstance*& EquipSlotItem, UCIS_EquipmentInstance*& Item);
	
	void SetUpAttachmentOnItem(UCIS_ItemInstance*& EquipSlot, UCIS_ItemDefinition*& AttachmentDefinition);
	UCIS_ItemDefinition* GetAndRemoveAttachmentFromSlot(UCIS_ItemInstance*& EquipSlot/*, UCIS_ItemCategory* Category*/);
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Inventory|InventorySlots")
	TArray<UCIS_ItemInstance*> GetInventorySlots() const;
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_InventorySlots)
	TArray<UCIS_ItemInstance*> InventorySlots;
};
