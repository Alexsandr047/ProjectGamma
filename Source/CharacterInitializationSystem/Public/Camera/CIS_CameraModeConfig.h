// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CIS_CameraMode.h"
#include "Engine/DataAsset.h"
#include "CIS_CameraModeConfig.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_CameraModeConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UCIS_CameraModeConfig(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Meta = (TitleProperty = "Camera Configs"))
	FCIS_FirstAndThirdMode CameraModes;
	
	const FCIS_FirstAndThirdMode GetFirstAndThirdMode() const
	{
		return CameraModes;
	}
};
