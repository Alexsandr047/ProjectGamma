// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineBeacons/PG_OnlineBeaconClient.h"

APG_OnlineBeaconClient::APG_OnlineBeaconClient()
{
}

void APG_OnlineBeaconClient::OnFailure()
{
	UE_LOG(LogTemp, Warning, TEXT("Clent failed to connect to HOST Beacon"));
	OnConnectSuccess.Broadcast(false);
	//Super::OnFailure();
}

void APG_OnlineBeaconClient::OnConnected()
{
	UE_LOG(LogTemp, Warning, TEXT("Clent to connect to HOST Beacon"));
	OnConnectSuccess.Broadcast(true);
	//Super::OnConnected();
}

void APG_OnlineBeaconClient::Client_OnDisconnected_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Clent Disconected"));
	OnDisconnected.Broadcast();
}

void APG_OnlineBeaconClient::Client_FullConnect_Implementation()
{
	LeaveLobby();
	OnFullConnect.Broadcast();
}

void APG_OnlineBeaconClient::FullConnectToServer(const FString& JoinAddress)
{
	if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
	}
	LeaveLobby();
}

bool APG_OnlineBeaconClient::ConnectToServer(const FString& Address)
{
	FURL Destination = FURL(nullptr, *Address, ETravelType::TRAVEL_Absolute);
	Destination.Port = 7787;
	return InitClient(Destination);
}

void APG_OnlineBeaconClient::LeaveLobby()
{
	DestroyBeacon();
}

