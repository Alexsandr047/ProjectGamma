// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_PlayerData.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTGAMMA_API UPG_PlayerData : public UObject
{
	GENERATED_BODY()

public:

	UPG_PlayerData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//TArray<FModifiableItemBase>, PlayerItems, TArray<FEquippedItemBase>, EquipItems
};