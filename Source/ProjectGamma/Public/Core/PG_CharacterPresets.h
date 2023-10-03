// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CIS_Character.h"
#include "Engine/DataAsset.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"
#include "PG_CharacterPresets.generated.h"


/**
 * 
 */
/*USTRUCT(BlueprintType)
struct FClothesPresets
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EPG_ClothesType ClothType;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCIS_ItemDefinition> ClothDefinition;
};*/

USTRUCT(BlueprintType)
struct FCharacterPresets
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* UpdateSkeletalMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> UpdateAnimInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> UpdateAnimLayersInstanceClass;
};

UCLASS(BlueprintType, Const, Meta = (DisplayName = "Character Preset", ShortTooltip = "Data asset containing Charcter Preset data."))
class PROJECTGAMMA_API UPG_CharacterPresets : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPG_CharacterPresets(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly)
	FCharacterPresets CharacterPresets;
};
