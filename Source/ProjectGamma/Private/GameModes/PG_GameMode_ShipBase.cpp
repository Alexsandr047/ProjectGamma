// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/PG_GameMode_ShipBase.h"

#include "InventorySystem/PG_ItemsSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Map/Hub/PG_Hub_RoomsSubsystem.h"
#include "OnlineBeaconHost.h"
#include "OnlineBeacons/PG_OnlineBeaconHostObject.h"
#include "Player/PG_PlayerController.h"
#include "Player/PG_ShipBasePlayerController.h"

APG_GameMode_ShipBase::APG_GameMode_ShipBase()
{
	HostObject = nullptr;
}

void APG_GameMode_ShipBase::PostLogin(APlayerController* NewPlayer)
{		
	Super::PostLogin(NewPlayer);	
}

void APG_GameMode_ShipBase::OnPlayerInitializedClientData(UPG_QuickBarComponent* QuickBarComponent)
{
	UGameInstance* GI = GetGameInstance();

	UPG_Hub_RoomsSubsystem* RoomsSubsystem = GI->GetSubsystem<UPG_Hub_RoomsSubsystem>();
	check(RoomsSubsystem);

	FOnShipDataParsed ShipDataParsed;
	ShipDataParsed.AddDynamic(this, &ThisClass::SetParsedItemData);
	RoomsSubsystem->ReadJsonFileAndMakeShipData(Cast<APG_ShipBasePlayerController>(QuickBarComponent->GetPlayerController()), ShipDataParsed);	
}

void APG_GameMode_ShipBase::SetParsedItemData(APG_ShipBasePlayerController* PlayerController,
	FShipData ShipData)
{
	//MapGenerator = Cast<APG_Hub_MapGenerator>(UGameplayStatics::GetActorOfClass(this, APG_Hub_MapGenerator::StaticClass()));
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	MapGenerator = GetWorld()->SpawnActor<APG_Hub_MapGenerator>(MapGeneratorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if(MapGenerator)
	{
		MapGenerator->SetDateTo(ShipData.DateTo);
		MapGenerator->GenerateRooms(ShipData.ShipRooms);
		MapGenerator->InitTechTree(ShipData.Technologies);
	}
}

bool APG_GameMode_ShipBase::CreateHostBeacon()
{
	if(AOnlineBeaconHost* Host = GetWorld()->SpawnActor<AOnlineBeaconHost>(AOnlineBeaconHost::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("SPAWN AOnlineBeaconHost"))
		if(Host->InitHost())
		{
			Host->PauseBeaconRequests(false);
			UE_LOG(LogTemp, Warning, TEXT("Init Host"))
			HostObject = GetWorld()->SpawnActor<APG_OnlineBeaconHostObject>(APG_OnlineBeaconHostObject::StaticClass());
			if(HostObject)
			{
				UE_LOG(LogTemp, Warning, TEXT("SPAWNED Host Object"))
				Host->RegisterHost(HostObject);
				return true;
			}
		}		
	}
	return false;
}

APG_OnlineBeaconHostObject* APG_GameMode_ShipBase::GetBeaconHost() const
{
	return HostObject;
}
