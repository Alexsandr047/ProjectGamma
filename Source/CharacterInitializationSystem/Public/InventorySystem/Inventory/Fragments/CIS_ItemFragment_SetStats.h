// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CIS_GameplayTagStack.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"
#include "CIS_ItemFragment_SetStats.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_ItemFragment_SetStats : public UCIS_ItemFragment
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;

public:
	virtual void OnInstanceCreated(UCIS_ItemInstance* Instance) const override;
	void SetTagStats(FGameplayTag Tag, int32 Stat);
	int32 GetItemStatByTag(FGameplayTag Tag) const;
	TMap<FGameplayTag, int32> GetItemStats() const; 
};
