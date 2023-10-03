// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PG_AssetManager.h"

UPG_AssetManager& UPG_AssetManager::Get()
{
	check(GEngine);

	if (UPG_AssetManager* Singleton = Cast<UPG_AssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to CIS_AssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UPG_AssetManager>();
}

const UPG_ItemsData* UPG_AssetManager::GetItemsData()
{
	return GetAsset(PG_ItemData);
}

const UPG_CharacterPresets* UPG_AssetManager::GetCharacterPresets()
{
	return GetAsset(PG_CharacterPresets);
}
