// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Equipment/CIS_EquipmentInstance.h"
#include "AbilitySystem/Abilities/CIS_GameplayAbility.h"
#include "InventorySystem/Inventory/CIS_ItemManagerComponent.h"
#include "PG_GameplayAbility_FromEquipment.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_GameplayAbility_FromEquipment : public UCIS_GameplayAbility
{
	GENERATED_BODY()

public:

	UPG_GameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="PG|Ability")
	UCIS_EquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "PG|Ability")
	UCIS_ItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

	
};
