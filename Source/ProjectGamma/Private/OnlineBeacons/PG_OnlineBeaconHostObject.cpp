// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineBeacons/PG_OnlineBeaconHostObject.h"

#include "OnlineBeaconHost.h"
#include "GameModes/PG_GameMode_ShipBase.h"
#include "OnlineBeacons/PG_OnlineBeaconClient.h"

APG_OnlineBeaconHostObject::APG_OnlineBeaconHostObject()
{
	ClientBeaconActorClass = APG_OnlineBeaconClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();
}

void APG_OnlineBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor,
	UNetConnection* ClientConnection)
{
	Super::OnClientConnected(NewClientActor, ClientConnection);

	if(NewClientActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected Client Valid"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Connected Client Invalid"))
	}
}

void APG_OnlineBeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
{
	Super::NotifyClientDisconnected(LeavingClientActor);
	UE_LOG(LogTemp, Warning, TEXT("Client Has Disconnected"))
}

void APG_OnlineBeaconHostObject::DisconnectClient(AOnlineBeaconClient* ClientActor)
{
	//Super::DisconnectClient(ClientActor);
	if(AOnlineBeaconHost* BeaconHost = Cast<AOnlineBeaconHost>(GetOwner()))
	{
		if(APG_OnlineBeaconClient* Client = Cast<APG_OnlineBeaconClient>(ClientActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Disconnecting Client: %s"), *ClientActor->GetName());
			Client->Client_OnDisconnected();
		}
		BeaconHost->DisconnectClient(ClientActor);
	}
}

void APG_OnlineBeaconHostObject::ShutdownServer()
{
	DisconnectAllClient();

	if(AOnlineBeaconHost* Host = Cast<AOnlineBeaconHost>(GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy Beacon"));
		Host->UnregisterHost(BeaconTypeName);
		Host->DestroyBeacon();
	}
}
void APG_OnlineBeaconHostObject::DisconnectAllClient()
{
	UE_LOG(LogTemp, Warning, TEXT("Disconnect All Client"));
	for(auto& Client : ClientActors)
	{
		if(Client)
			DisconnectClient(Client);
	}
}

void APG_OnlineBeaconHostObject::StartServer(const FString& MapUrl)
{
	for(auto& Client : ClientActors)
	{
		if(APG_OnlineBeaconClient* BeaconClient = Cast<APG_OnlineBeaconClient>(Client))
		{
			BeaconClient->Client_FullConnect();
		}
	}
	ShutdownServer();
	GetWorld()->ServerTravel(MapUrl + "?Listen");
}
