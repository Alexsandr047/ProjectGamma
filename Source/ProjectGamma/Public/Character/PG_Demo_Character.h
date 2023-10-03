// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CIS_Character.h"
#include "PG_Demo_Character.generated.h"

/**
 * 
 */

class UPG_CharacterPresets_Component;

UCLASS()
class PROJECTGAMMA_API APG_Demo_Character : public ACIS_Character
{
	GENERATED_BODY()

public:

	APG_Demo_Character(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPG_CharacterPresets_Component* CharacterPresets;
	
};
