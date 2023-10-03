// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CIS_HUD.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_HUD : public AHUD
{
	GENERATED_BODY()

public:

	ACIS_HUD();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetActorInHud(AActor* Target);


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ClearActorInHud();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowInteractionPrompt(float InteractionDuration, AActor* Target);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideInteractionPrompt();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartInteractionTimer(float InteractionDuration, AActor* Target);
	
	// Interaction interrupted, cancel and hide HUD interact timer
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StopInteractionTimer();
};
