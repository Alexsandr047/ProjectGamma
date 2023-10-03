// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PG_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_InventoryItem : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void InitIconData(const FString& DescriptionItemID); 
};
