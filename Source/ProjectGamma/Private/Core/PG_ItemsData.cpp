// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PG_ItemsData.h"
#include "Core/CIS_AssetManager.h"
#include "Core/PG_AssetManager.h"

UPG_ItemsData::UPG_ItemsData(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	ItemsDataTable = nullptr;
	ItemsTagsDataTable = nullptr;
	DefaultItemPresets = nullptr;
}
