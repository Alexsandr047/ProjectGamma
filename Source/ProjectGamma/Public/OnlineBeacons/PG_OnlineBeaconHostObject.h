// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "PG_OnlineBeaconHostObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API APG_OnlineBeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()
	
	public:
	
	APG_OnlineBeaconHostObject();

protected:
	virtual void OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection) override;
	virtual void NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor) override;
	virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;
	
	UFUNCTION(BlueprintCallable)
	void ShutdownServer();
	void DisconnectAllClient();

	UFUNCTION(BlueprintCallable)
	void StartServer(const FString& MapUrl);
};
