// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "PG_OnlineBeaconClient.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectSuccess, bool, Connected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisconnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFullConnect);
UCLASS()
class PROJECTGAMMA_API APG_OnlineBeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()
	
public:
	
	APG_OnlineBeaconClient();

protected:
	UPROPERTY(BlueprintAssignable)
	FOnConnectSuccess OnConnectSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnDisconnected OnDisconnected;
	
	UPROPERTY(BlueprintAssignable)
	FOnFullConnect OnFullConnect;
	
	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool ConnectToServer(const FString& Address);
	UFUNCTION(BlueprintCallable)
	void LeaveLobby();
	
	virtual void OnFailure() override;
	virtual  void OnConnected() override;

public:
	UFUNCTION(Client, Reliable)
	void Client_OnDisconnected();
	virtual void Client_OnDisconnected_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_FullConnect();
	virtual void Client_FullConnect_Implementation();

 	UFUNCTION(BlueprintCallable)
	void FullConnectToServer(const FString& JoinAddress);
};
