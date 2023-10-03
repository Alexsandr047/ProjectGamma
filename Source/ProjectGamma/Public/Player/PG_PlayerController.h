// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PG_CoreTypes.h"
#include "InventorySystem/PG_ItemsSubsystem.h"
#include "Player/CIS_PlayerController.h"
#include "PG_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClientDataInitialized, class APG_PlayerController*, PlayerController);

struct FModifiableItemBase;
struct FEquippedItemBase;

UCLASS()
class PROJECTGAMMA_API APG_PlayerController : public ACIS_PlayerController
{
	GENERATED_BODY()

public:
	APG_PlayerController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	EPlayerDataType PlayerTypeData;
	// Broadcast on server, when receive initializing data from client
	UPROPERTY()
	FClientDataInitialized OnClientDataInitialized;

	UFUNCTION()
	void SetParsedPlayerData(UPG_PlayerData* PlayerData);
	UPG_PlayerData* GetPlayerData() { return PlayerData; }
	
protected:
	virtual void BeginPlay() override;	
	virtual void OnPossess(APawn* Pawn) override;

	UPROPERTY()
	UPG_PlayerData* PlayerData;
		
	UFUNCTION(Server, Reliable)
	void InitializeClientData();	
};
