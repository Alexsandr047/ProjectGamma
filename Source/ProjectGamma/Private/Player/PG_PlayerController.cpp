// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PG_PlayerController.h"
#include "Core/PG_AssetManager.h"
#include "GameModes/PG_ProceduralWorld_GameMode.h"
#include "Kismet/GameplayStatics.h"

APG_PlayerController::APG_PlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PlayerTypeData = EPlayerDataType::ShipBase;
}

void APG_PlayerController::SetParsedPlayerData(UPG_PlayerData* _PlayerData)
{
	PlayerData = _PlayerData;
	
	if(ACIS_Character* CurrentPawn = Cast<ACIS_Character>(GetPawn()))
		CurrentPawn->InitCharacterData();
}

void APG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*if (IsLocalController() /*&& GetLocalRole() != ENetRole::ROLE_Authorit#1#)
	{
		SendClientData();
	}*/
}

void APG_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		InitializeClientData();
	}	
}

void APG_PlayerController::InitializeClientData_Implementation()
{
	/*ASRBasePlayerState* PS = GetPlayerState<ASRBasePlayerState>();
	PS->NakamaPlayerID = InPlayerNakamaID;*/

	OnClientDataInitialized.Broadcast(this);
}
