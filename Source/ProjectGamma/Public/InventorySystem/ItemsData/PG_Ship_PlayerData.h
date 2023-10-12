// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_PlayerData.h"
#include "Map/Hub/PG_Hub_MapGenerator.h"
#include "PG_Ship_PlayerData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_Ship_PlayerData : public UPG_PlayerData
{
	GENERATED_BODY()

public:
	
	UPG_Ship_PlayerData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FShipData ShipData;	
};
