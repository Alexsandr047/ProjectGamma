// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CIS_GameData.h"

#include "Core/CIS_AssetManager.h"

UCIS_GameData::UCIS_GameData()
{
	
}

const UCIS_GameData& UCIS_GameData::Get()
{
	return UCIS_AssetManager::Get().GetGameData();
}
