// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_PlayerController.h"
#include "PG_ShipBasePlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClientShipDataInitialized, class APG_ShipBasePlayerController*, PlayerController);

UCLASS()
class PROJECTGAMMA_API APG_ShipBasePlayerController : public APG_PlayerController
{
	GENERATED_BODY()

public:

	APG_ShipBasePlayerController(const FObjectInitializer& ObjectInitializer);
	//FOnClientShipDataInitialized OnClientShipDataInitialized;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
