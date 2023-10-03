// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameMode.h"
#include "InventorySystem/PG_ItemsSubsystem.h"
#include "PG_GameMode.generated.h"


//class UCIS_HealthComponent;
class APG_PlayerController;
struct FModifiableItemBase;
struct FEquippedItemBase;

UCLASS()
class PROJECTGAMMA_API APG_GameMode : public AGameMode
{
	GENERATED_BODY()

public:

	APG_GameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// Call when Player have initialized clients data on server
	UFUNCTION(BlueprintCallable)
	void StartPlayerInitializedClientData(APlayerController* NewPlayer);
	UFUNCTION()
	virtual void OnPlayerInitializedClientData(APG_PlayerController* Player);
	
	// Restart (respawn) the specified player or bot next frame
	// - If bForceReset is true, the controller will be reset this frame (abandoning the currently possessed pawn, if any)
	UFUNCTION(BlueprintCallable)
	void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset = false);

	UFUNCTION(BlueprintCallable)
	void CharacterDied(AController* Controller);
protected:
	float RespawnDelay;

	UFUNCTION()
	void RespawnCharacter(AController* Controller);
	
private:
	UPROPERTY()
	bool bPlayerInitializedClientData;
};
