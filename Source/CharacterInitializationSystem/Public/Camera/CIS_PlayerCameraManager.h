// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CIS_PlayerCameraManager.generated.h"

#define CIS_CAMERA_DEFAULT_FOV			(80.0f)
#define CIS_CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define CIS_CAMERA_DEFAULT_PITCH_MAX	(89.0f)

class UCIS_UICameraManagerComponent;

/**
 * ACIS_PlayerCameraManager
 *
 *	The base player camera manager class used by this project.
 */

UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ACIS_PlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	UCIS_UICameraManagerComponent* GetUICameraComponent() const;

protected:

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

private:
	/** The UI Camera Component, controls the camera when UI is doing something important that gameplay doesn't get priority over. */
	UPROPERTY(Transient)
	UCIS_UICameraManagerComponent* UICamera;
};
