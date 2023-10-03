// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PG_ProjectGamma_Settings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Project Gamma Setting"))
class PROJECTGAMMA_API UPG_ProjectGamma_Settings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPG_ProjectGamma_Settings();
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Project Gamma Setting|General")
	bool UseBackEnd;
	
};
