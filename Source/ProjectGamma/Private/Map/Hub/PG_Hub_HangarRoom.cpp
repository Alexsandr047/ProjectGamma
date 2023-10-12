// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Hub/PG_Hub_HangarRoom.h"

#include "InventorySystem/PG_PlayerDataSubsystem.h"

// Sets default values
APG_Hub_HangarRoom::APG_Hub_HangarRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

/*
bool APG_Hub_HangarRoom::CreateSquad(FSquad Squad)
{

	return false;
}

bool APG_Hub_HangarRoom::DeleteSquad(const FString& SquadId)
{
	return false;
}

void APG_Hub_HangarRoom::ChangeSquad(const FString& SquadId, FSquad Squad)
{
	
}
*/

// Called when the game starts or when spawned
void APG_Hub_HangarRoom::BeginPlay()
{
	Super::BeginPlay();

	/*UGameInstance* GI = GetGameInstance();
	check(GI);
	
	UPG_PlayerDataSubsystem* PlayerDataSubsystem = GI->GetSubsystem<UPG_PlayerDataSubsystem>();
	check(PlayerDataSubsystem)
	PlayerDataSubsystem->GetSquadsData(Squads);*/
}
