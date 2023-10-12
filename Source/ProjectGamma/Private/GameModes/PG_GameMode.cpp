// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/PG_GameMode.h"

#include "Character/CIS_HealthComponent.h"
#include "Character/PG_Character.h"
#include "Core/PG_GameInstance.h"
#include "GameFramework/SpectatorPawn.h"
#include "InventorySystem/PG_PlayerDataSubsystem.h"
#include "InventorySystem/PG_QuickBarComponent.h"
#include "Player/PG_PlayerController.h"

APG_GameMode::APG_GameMode()
{
	RespawnDelay = 8.0f;
	bPlayerInitializedClientData = true;
}

void APG_GameMode::PostLogin(APlayerController* NewPlayer)
{
	StartPlayerInitializedClientData(NewPlayer);
	Super::PostLogin(NewPlayer);
}

void APG_GameMode::StartPlayerInitializedClientData(APlayerController* NewPlayer)
{
	UPG_QuickBarComponent* QuickBarComponent = NewPlayer->FindComponentByClass<UPG_QuickBarComponent>();
	if(QuickBarComponent)
		QuickBarComponent->OnClientDataInitialized.AddDynamic(this, &APG_GameMode::OnPlayerInitializedClientData);
}

void APG_GameMode::OnPlayerInitializedClientData(UPG_QuickBarComponent* QuickBarComponent)
{
	if(UPG_GameInstance * GI = Cast<UPG_GameInstance>(GetGameInstance()))
		GI->GetPlayerData(QuickBarComponent);
}

void APG_GameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if (bForceReset && (Controller != nullptr))
	{
		Controller->Reset();
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC, &APlayerController::ServerRestartPlayer_Implementation);
	}
}

void APG_GameMode::CharacterDied(AController* Controller)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParameters);

	Controller->UnPossess();
	Controller->Possess(SpectatorPawn);

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;

	RespawnDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::RespawnCharacter, Controller);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);

	//Set Respawn Countdown for HUD
	/*ACIS_PlayerController* PC = Cast<ACIS_PlayerController>(Controller);
	if (PC)
	{
		PC->SetRespawnCountdown(RespawnDelay);
	}*/
}

void APG_GameMode::RespawnCharacter(AController* Controller)
{
	if (!IsValid(Controller))
	{
		return;
	}

	if (Controller->IsPlayerController())
	{
		// Respawn player hero
		AActor* PlayerStart = FindPlayerStart(Controller);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APG_Character* Hero = GetWorld()->SpawnActor<APG_Character>(DefaultPawnClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), SpawnParameters);

		APawn* OldSpectatorPawn = Controller->GetPawn();
		Controller->UnPossess();
		OldSpectatorPawn->Destroy();
		Controller->Possess(Hero);
	}
}


