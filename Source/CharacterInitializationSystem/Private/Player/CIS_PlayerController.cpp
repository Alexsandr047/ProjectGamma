// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CIS_PlayerController.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/ControllerComponent.h"
#include "Player/CIS_PlayerState.h"

void ACIS_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	/*if(GetPawn())
	{
		GetHUD<ALibertyGameHUD>()->CreateMainWidget();
	}*/
}

ACIS_PlayerController::ACIS_PlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

ACIS_PlayerState* ACIS_PlayerController::GetCIS_PlayerState() const
{
	return CastChecked<ACIS_PlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UCIS_AbilitySystemComponent* ACIS_PlayerController::GetLiberty_AbilitySystemComponent() const
{
	const ACIS_PlayerState* CIS_PS = GetCIS_PlayerState();
	return (CIS_PS ? CIS_PS->GetCIS_AbilitySystemComponent() : nullptr);
}

void ACIS_PlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ACIS_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	
	Super::EndPlay(EndPlayReason);
}

void ACIS_PlayerController::ReceivedPlayer()
{
	// Player controllers always get assigned a player and can't do much until then
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	
	Super::ReceivedPlayer();

	TArray<UControllerComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for (UControllerComponent* Component : ModularComponents)
	{
		Component->ReceivedPlayer();
	}
}

void ACIS_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TArray<UControllerComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for (UControllerComponent* Component : ModularComponents)
	{
		Component->PlayerTick(DeltaTime);
	}

	// If we are auto running then add some player input
	if (GetIsAutoRunning())
	{
		if (APawn* CurrentPawn = GetPawn())
		{
			const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			CurrentPawn->AddMovementInput(MovementDirection, 1.0f);	
		}
	}
}

void ACIS_PlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ACIS_PlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UCIS_AbilitySystemComponent* LibertyASC = GetLiberty_AbilitySystemComponent())
	{
		LibertyASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

bool ACIS_PlayerController::GetIsAutoRunning() const
{
	bool bIsAutoRunning = false;
	/*if (const ULiberty_AbilitySystemComponent* LibertyASC = ULiberty_AbilitySystemComponent())
	{
		bIsAutoRunning = LibertyASC->GetTagCount(FLiberty_GameplayTags::Get().Status_AutoRunning) > 0;
	}*/
	return bIsAutoRunning;
}
