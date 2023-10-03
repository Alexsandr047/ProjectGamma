// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CIS_HeroCharacter.h"
#include "Core/PG_CharacterPresets.h"
#include "PG_Character.generated.h"

/**
 * 
 */
class UPG_CharacterPresets_Component;

UCLASS()
class PROJECTGAMMA_API APG_Character : public ACIS_HeroCharacter
{
	GENERATED_BODY()

public:

	APG_Character(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPG_CharacterPresets_Component* CharacterPresets;
};
