// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/PG_ProceduralWorld_GameMode.h"
#include "Map/PG_MapGenerator.h"
#include "Map/PG_RoomGenerator.h"


APG_ProceduralWorld_GameMode::APG_ProceduralWorld_GameMode()
{
	WorldIsGenerated = false;
}

void APG_ProceduralWorld_GameMode::PostLogin(APlayerController* NewPlayer)
{	
	AGameMode::PostLogin(NewPlayer);
}

void APG_ProceduralWorld_GameMode::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
		WorldGenerate();
}

void APG_ProceduralWorld_GameMode::WorldGenerate()
{
	RoomGenerator = GetWorld()->SpawnActor<APG_RoomGenerator>(RoomGeneratorClass, FVector(), FRotator (0.f));
	if(RoomGenerator)
	{
		RoomGenerator->Init(RoomList);
	}
}
