// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CIS_VerbMessage.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CIS_VerbMessageHelpers.generated.h"

class APlayerState;
class APlayerController;
struct FGameplayCueParameters;

UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_VerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CIS")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "CIS")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "CIS")
	static FGameplayCueParameters VerbMessageToCueParameters(const FCIS_VerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "CIS")
	static FCIS_VerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);

	
};
