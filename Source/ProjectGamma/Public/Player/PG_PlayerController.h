// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PG_CoreTypes.h"
#include "InventorySystem/PG_ItemsSubsystem.h"
#include "Player/CIS_PlayerController.h"
#include "PG_PlayerController.generated.h"

struct FModifiableItemBase;
struct FEquippedItemBase;

UCLASS()
class PROJECTGAMMA_API APG_PlayerController : public ACIS_PlayerController
{
	GENERATED_BODY()

public:
	APG_PlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	
	virtual void BeginPlay() override;	
	virtual void OnPossess(APawn* Pawn) override;
};
