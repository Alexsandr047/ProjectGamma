// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/PG_GameMode.h"
#include "PG_ProceduralWorld_GameMode.generated.h"

/**
 * 
 */
class APG_RoomGenerator;
class UPG_Base_RoomList;

UCLASS()
class PROJECTGAMMA_API APG_ProceduralWorld_GameMode : public APG_GameMode
{
	GENERATED_BODY()

public:
	
	APG_ProceduralWorld_GameMode();
	
	bool WorldIsGenerated;
	
	// Players that have initialized clients data on server
	int32 NumInitializedPlayers = 0;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<APlayerController*> ConnectedPlayers;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	UPG_Base_RoomList* RoomList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomGenerator> RoomGeneratorClass;
	UPROPERTY()
	APG_RoomGenerator* RoomGenerator;
	
	void WorldGenerate();
	
};
