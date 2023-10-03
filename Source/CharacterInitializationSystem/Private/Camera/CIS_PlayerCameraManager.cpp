

#include "Camera/CIS_PlayerCameraManager.h"

#include "Camera/CIS_CameraComponent.h"
#include "Camera/CIS_UICameraManagerComponent.h"
#include "Engine/Canvas.h"

static FName UICameraComponentName(TEXT("UICamera"));

ACIS_PlayerCameraManager::ACIS_PlayerCameraManager(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	DefaultFOV = CIS_CAMERA_DEFAULT_FOV;
	ViewPitchMin = CIS_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = CIS_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UCIS_UICameraManagerComponent>(UICameraComponentName);
}

UCIS_UICameraManagerComponent* ACIS_PlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void ACIS_PlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}
	
	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void ACIS_PlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL,
	float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("LibertyPlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UCIS_CameraComponent* CameraComponent = UCIS_CameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}
