// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/PG_ItemsSubsystem.h"
#include "PG_Character_PlayerData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_Character_PlayerData : public UPG_PlayerData
{
	GENERATED_BODY()

public:
	
	UPG_Character_PlayerData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FString CharacterName;
	
	TArray<FEquippedItemBase> EquipItems;

};
