// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PG_UtilityFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_UtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	static bool GetUseBackEnd();
};
