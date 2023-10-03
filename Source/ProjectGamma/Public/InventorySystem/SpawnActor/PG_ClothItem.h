// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Inventory/SpawActor/CIS_EquipmentItem.h"
#include "PG_ClothItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API APG_ClothItem : public ACIS_EquipmentItem
{
	GENERATED_BODY()
	
public:
	
	APG_ClothItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	virtual void OnRep_OwningCharacter() override;
};
