// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "PG_ItemsData.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Const, Meta = (DisplayName = "Items Data", ShortTooltip = "Data asset containing Item Defenitions data."))
class PROJECTGAMMA_API UPG_ItemsData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPG_ItemsData(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditDefaultsOnly, Category = "", meta = (DisplayName = ""))
	UDataTable* ItemsDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "", meta = (DisplayName = ""))
	UDataTable* ItemsTagsDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "", meta = (DisplayName = ""))
	UDataTable* DefaultItemPresets;
};
