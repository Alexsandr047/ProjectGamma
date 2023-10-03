// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PG_CoreTypes.h"
#include "InventorySystem/Equipment/CIS_QuickBarComponent.h"
#include "PG_QuickBarComponent.generated.h"

/**
 * 
 */


class APG_PlayerController;

UENUM(BlueprintType)
enum class EPG_EWeaponSlots : uint8
{
	Default,
	PrimaryWeaponSlot,	
	SpecialWeaponSlot,	
	PistolWeaponSlots,
};



UENUM(BlueprintType)
enum class EPG_EquipmentType : uint8
{
	Default,
	Helmet		UMETA(DisplayName="Helmet"),
	Armor		UMETA(DisplayName="Armor"),
};

USTRUCT(BlueprintType)
struct FPG_GetItemsInInventoryMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TArray<UCIS_ItemInstance*> Items;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemLoaded);
DECLARE_DELEGATE_OneParam(FOnPreparingItemDefinitions, const TArray<UCIS_ItemDefinition*>&);
//DECLARE_DELEGATE_OneParam(FOnEventDelegate,  const TArray<UCIS_ItemDefinition*>&);

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class PROJECTGAMMA_API UPG_QuickBarComponent : public UCIS_QuickBarComponent
{
	GENERATED_BODY()

public:
	UPG_QuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	APG_PlayerController* GetPlayerController() const;
	
	UFUNCTION()
	void OnRep_SlotChanged();
	
	UPROPERTY(ReplicatedUsing=OnRep_SlotChanged)
	UCIS_ItemInstance* HelmetSlot;
	UPROPERTY(ReplicatedUsing=OnRep_SlotChanged)
	UCIS_ItemInstance* ArmorSlot;
	UPROPERTY()
	UCIS_EquipmentInstance* HelmetItem;
	UPROPERTY()
	UCIS_EquipmentInstance* ArmorItem;

	UPROPERTY(ReplicatedUsing=OnRep_SlotChanged)
	UCIS_ItemInstance* PrimaryWeaponSlot;
	UPROPERTY(Replicated)
	UCIS_EquipmentInstance* PrimaryWeaponItem;	

	UPROPERTY(ReplicatedUsing=OnRep_SlotChanged)
	UCIS_ItemInstance* SpecialWeaponSlot;
	UPROPERTY(Replicated)
	UCIS_EquipmentInstance* SpecialWeaponItem;

	UPROPERTY(ReplicatedUsing=OnRep_SlotChanged)
	UCIS_ItemInstance* PistolWeaponSlot;
	UPROPERTY(Replicated)
	UCIS_EquipmentInstance* PistolWeaponItem;
	
	UPROPERTY(BlueprintAssignable)
	FOnEquipUpdate UpdateEquipSlot;
	
	//Armor Equip Logic
	UFUNCTION(BlueprintAuthorityOnly)
	void EquipEquipmentTypeOnCharacter(UCIS_ItemDefinition* Definition, EPG_EquipmentType EquipmentType);
	//
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void EquipWeaponInWeaponSlot(UCIS_ItemDefinition* Definition, EPG_EWeaponSlots WeaponSlots);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UseItemOnEquipmentSlot(EPG_EWeaponSlots EquipmentsSlots, int32 InventorySlot);
	
	UFUNCTION(BlueprintCallable)
	void GetWeaponItem(EPG_EWeaponSlots EquipmentsSlots, UCIS_EquipmentInstance*& Weapon);

	UFUNCTION(BlueprintCallable)
	void GetCurrentlyInUseWeapon(UCIS_EquipmentInstance*& Weapon);
	
	UFUNCTION(BlueprintCallable)
	void GetWeaponSlots(EPG_EWeaponSlots EquipmentsSlots, UCIS_ItemInstance*& WeaponSlot);
	
	UFUNCTION(BlueprintCallable)
	void GetItemsInInventory();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RemoveAllItemsFromSlots();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool AddItemInEmptySlotInventory(UCIS_ItemInstance* Item = nullptr);
	
	// Init Items for player Pawn, when pawn spawned.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void InitEquip(UCIS_ItemManagerComponent* ItemManager);
	/*void InitEquipAndInventory(UCIS_ItemManagerComponent* ItemManager);*/
	void PreparingItemDefinitions(TArray<FModifiableItemBase> ParsedItems, FOnPreparingItemDefinitions OnPreparingItemDefinitions);
	UPROPERTY(BlueprintAssignable)
	FOnItemLoaded ItemLoaded;
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void InitItemsToInventory(TArray<FModifiableItemBase> Items);
	
	UPROPERTY()
	UCIS_ItemManagerComponent* ItemManager;

};
