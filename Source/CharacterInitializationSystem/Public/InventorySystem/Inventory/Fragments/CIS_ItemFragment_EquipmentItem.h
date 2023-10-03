// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"
#include "GameplayTagContainer.h"
#include "CIS_ItemFragment_EquipmentItem.generated.h"


class UCIS_EquipmentInstance;
class UCIS_AbilitySet;
class UCIS_AttachmentLogic;
class ACIS_EquipmentItem;
class ACIS_AttachmentItem;
/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_ItemFragment_EquipmentItem : public UCIS_ItemFragment
{
	GENERATED_BODY()

public:
		
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UCIS_EquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, TObjectPtr<const UCIS_AbilitySet>> DefaultAbility;

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<ACIS_EquipmentItem> ActorToSpawn;

	UPROPERTY(EditDefaultsOnly,Category=Equipment)
	TSubclassOf<UCIS_AttachmentLogic> AttachmentLogic;
	
	UFUNCTION()
	void SetAttachmentDefinition(TArray<UCIS_ItemDefinition*> ItemDefinitions);
	
	UFUNCTION()
	TArray<FGameplayTag> InitAttachableFragments(UObject* Outer, UCIS_ItemInstance* Instance);
	
	UPROPERTY()
	TArray<UCIS_ItemDefinition*> AttachmentDefinition;
	
	TArray<TSubclassOf<ACIS_AttachmentItem>> GetAttachmentItem() const;
	TArray<TObjectPtr<const UCIS_AbilitySet>> GetAbilitySets() const;
};
