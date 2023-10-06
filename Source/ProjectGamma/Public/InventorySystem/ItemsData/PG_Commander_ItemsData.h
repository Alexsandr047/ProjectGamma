// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_Character_PlayerData.h"
#include "Core/PG_CoreTypes.h"
#include "PG_Commander_ItemsData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_Commander_ItemsData : public UPG_Character_PlayerData
{
	GENERATED_BODY()

public:

	UPG_Commander_ItemsData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
