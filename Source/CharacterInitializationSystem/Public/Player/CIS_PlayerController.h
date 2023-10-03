// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CIS_PlayerController.generated.h"

class UCIS_AbilitySystemComponent;
class ACIS_PlayerState;
class ACIS_HUD;
/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void OnRep_Pawn() override;

public:

	ACIS_PlayerController(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "CharacterInitializationSystem|PlayerController")
	ACIS_PlayerState* GetCIS_PlayerState() const;
	
	UFUNCTION(BlueprintCallable, Category = "CharacterInitializationSystem|PlayerController")
	UCIS_AbilitySystemComponent* GetLiberty_AbilitySystemComponent() const;

	
	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface

	//~ Begin APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~ End APlayerController interface
	

	/** Called when the local player is assigned a player controller */
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, ULocalPlayer* LocalPlayer, APlayerController* PlayerController);
	FPlayerControllerSetDelegate OnPlayerControllerSet;

	/** Called when the local player is assigned a player state */
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStateSetDelegate, ULocalPlayer* LocalPlayer, APlayerState* PlayerState);
	FPlayerStateSetDelegate OnPlayerStateSet;

	/** Called when the local player is assigned a player pawn */
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerPawnSetDelegate, ULocalPlayer* LocalPlayer, APawn* Pawn);
	FPlayerPawnSetDelegate OnPlayerPawnSet;
	
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
	UFUNCTION(BlueprintCallable, Category = "CharacterInitializationSystem|Character")
	bool GetIsAutoRunning() const;
	
};
	

