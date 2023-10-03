// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CIS_UICameraManagerComponent.h"

#include "Camera/CIS_PlayerCameraManager.h"
#include "GameFramework/HUD.h"


UCIS_UICameraManagerComponent* UCIS_UICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if (PC != nullptr)
	{
		if (ACIS_PlayerCameraManager* PCCamera = Cast<ACIS_PlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}

	return nullptr;
}

UCIS_UICameraManagerComponent::UCIS_UICameraManagerComponent()
{
	bWantsInitializeComponent = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		// Register "showdebug" hook.
		if (!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
		}
	}
}

void UCIS_UICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UCIS_UICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<ACIS_PlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UCIS_UICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UCIS_UICameraManagerComponent::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	
}

void UCIS_UICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo,
	float& YL, float& YPos)
{
	
}


