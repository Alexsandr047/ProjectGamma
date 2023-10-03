// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CIS_GameplayTagStack.h"
#include "GameplayTagContainer.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"
#include "UObject/NoExportTypes.h"
#include "CIS_CoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FSubclassOfObject
{
	GENERATED_BODY()

	UPROPERTY()
	FString Key;
	UPROPERTY()
	TSubclassOf<UObject> ItemDefinition;
	
};

DECLARE_DELEGATE_OneParam(FAsyncLoadObjectDelegate, const TArray<UObject*>&);
DECLARE_DELEGATE_OneParam(FAsyncLoadClassObjectDelegate, const TArray<TSubclassOf<UObject>>&);
DECLARE_DELEGATE_OneParam(FAsyncLoadMapClassObjectDelegate, const  FSubclassOfObject&);

UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_CoreTypes : public UObject
{
	GENERATED_BODY()
	
};
