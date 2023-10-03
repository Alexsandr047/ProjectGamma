// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_CharacterPresets.h"
#include "PG_ItemsData.h"
#include "Core/CIS_AssetManager.h"
#include "PG_AssetManager.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class PROJECTGAMMA_API UPG_AssetManager : public UCIS_AssetManager
{
	GENERATED_BODY()

public:
	
	// Returns the AssetManager singleton object.
	static UPG_AssetManager& Get();
	
	const UPG_ItemsData* GetItemsData();

	const UPG_CharacterPresets* GetCharacterPresets();
protected:
	
	// Global game data asset to use.
	UPROPERTY(Config)
	TSoftObjectPtr<UPG_ItemsData> PG_ItemData;

	// Global game data asset to use.
	UPROPERTY(Config)
	TSoftObjectPtr<UPG_CharacterPresets> PG_CharacterPresets;
};
