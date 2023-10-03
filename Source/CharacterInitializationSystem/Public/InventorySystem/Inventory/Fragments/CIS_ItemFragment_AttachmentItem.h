// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"
#include "CIS_ItemFragment_AttachmentItem.generated.h"


class UCIS_EquipmentInstance;
class UCIS_AbilitySet;
class ACIS_AttachmentItem;

/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_ItemFragment_AttachmentItem : public UCIS_ItemFragment
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<ACIS_AttachmentItem> ActorToSpawn;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TObjectPtr<const UCIS_AbilitySet> AbilitySetsToGrant;
};
