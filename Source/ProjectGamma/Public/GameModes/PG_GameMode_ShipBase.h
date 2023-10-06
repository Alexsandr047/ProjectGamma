// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_GameMode.h"
#include "Core/PG_CoreTypes.h"
#include "Map/Hub/PG_Hub_MapGenerator.h"
#include "PG_GameMode_ShipBase.generated.h"

/**
 * 
 */

class APG_ShipBasePlayerController;
class APG_PlayerController;
class APG_OnlineBeaconHostObject;
class AOnlineBeaconHost;
class APG_Hub_MapGenerator;


UCLASS()
class PROJECTGAMMA_API APG_GameMode_ShipBase : public APG_GameMode
{
	GENERATED_BODY()
	
public:
	
	APG_GameMode_ShipBase();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY()
	APG_Hub_MapGenerator* MapGenerator = nullptr;
	
	virtual void OnPlayerInitializedClientData(UPG_QuickBarComponent* QuickBarComponent) override;

	UFUNCTION()
	void SetParsedItemData(APG_ShipBasePlayerController* PlayerController, FShipData ShipData);
	
private:

protected:

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CreateHostBeacon();

	UPROPERTY()
	APG_OnlineBeaconHostObject* HostObject;

	UFUNCTION(BlueprintCallable)
	APG_OnlineBeaconHostObject* GetBeaconHost() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectGamma|Room Generator")
	TSubclassOf<APG_Hub_MapGenerator> MapGeneratorClass;
};

