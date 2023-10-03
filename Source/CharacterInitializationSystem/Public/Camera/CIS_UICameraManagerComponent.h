// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CIS_UICameraManagerComponent.generated.h"


class APlayerController;
class ALiberty_PlayerCameraManager;
class AHUD;
class UCanvas;

UCLASS( Transient, Within=CIS_PlayerCameraManager )
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_UICameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static UCIS_UICameraManagerComponent* GetComponent(APlayerController* PC);

public:
	UCIS_UICameraManagerComponent();	
	virtual void InitializeComponent() override;

	bool IsSettingViewTarget() const { return bUpdatingViewTarget; }
	AActor* GetViewTarget() const { return ViewTarget; }
	void SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams());

	bool NeedsToUpdateViewTarget() const;
	void UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime);

	void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);

private:
	UPROPERTY(Transient)
	AActor* ViewTarget;
	
	UPROPERTY(Transient)
	bool bUpdatingViewTarget;
		
};
