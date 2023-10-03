// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CIS_PlayerState.h"

#include "AbilitySystem/AttributeSet/CIS_CombatSet.h"
#include "AbilitySystem/AttributeSet/CIS_HealthSet.h"
#include "Character/CIS_PawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/PlayerStateComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "UI/CIS_HUD.h"


ACIS_PlayerState::ACIS_PlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
, MyPlayerConnectionType(ECIS_PlayerConnectionType::Player)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UCIS_AbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CreateDefaultSubobject<UCIS_HealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UCIS_CombatSet>(TEXT("CombatSet"));
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void ACIS_PlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ACIS_PlayerState::Reset()
{
	Super::Reset();
}

void ACIS_PlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	/*if (UCIS_PawnExtensionComponent* PawnExtComp = UCIS_PawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}*/
}

void ACIS_PlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	//@TODO: Copy stats
}

void ACIS_PlayerState::OnDeactivated()
{
	bool bDestroyDeactivatedPlayerState = false;

	switch (GetPlayerConnectionType())
	{
	case ECIS_PlayerConnectionType::Player:
	case ECIS_PlayerConnectionType::InactivePlayer:
		//@TODO: Ask the experience if we should destroy disconnecting players immediately or leave them around
		// (e.g., for long running servers where they might build up if lots of players cycle through)
		bDestroyDeactivatedPlayerState = true;
		break;
	default:
		bDestroyDeactivatedPlayerState = true;
		break;
	}
	
	SetPlayerConnectionType(ECIS_PlayerConnectionType::InactivePlayer);

	if (bDestroyDeactivatedPlayerState)
	{
		Destroy();
	}
}

void ACIS_PlayerState::OnReactivated()
{
	if (GetPlayerConnectionType() == ECIS_PlayerConnectionType::InactivePlayer)
	{
		SetPlayerConnectionType(ECIS_PlayerConnectionType::Player);
	}
}
void ACIS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyPlayerConnectionType, SharedParams)
	/*DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MySquadID, SharedParams);*/

	DOREPLIFETIME(ThisClass, StatTags);
}

ACIS_PlayerController* ACIS_PlayerState::GetCIS_PlayerController() const
{
	return Cast<ACIS_PlayerController>(GetOwner());
}

UAbilitySystemComponent* ACIS_PlayerState::GetAbilitySystemComponent() const
{
	return GetCIS_AbilitySystemComponent();
}

void ACIS_PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	
}

/*void ACIS_PlayerState::OnRep_PawnData()
{
}*/

void ACIS_PlayerState::SetPlayerConnectionType(ECIS_PlayerConnectionType NewType)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyPlayerConnectionType, this);
	MyPlayerConnectionType = NewType;
}

void ACIS_PlayerState::SetSquadID(int32 NewSquadId)
{
	if (HasAuthority())
	{
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MySquadID, this);

		MySquadID = NewSquadId;
	}
}

/*
void ACIS_PlayerState::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = MyTeamID;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
		MyTeamID = NewTeamID;
		ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot set team for %s on non-authority"), *GetPathName(this));
	}
}
*/

/*FGenericTeamId ACIS_PlayerState::GetGenericTeamId() const
{
	return MyTeamID;
}*/

/*
FOnLyraTeamIndexChangedDelegate* ACIS_PlayerState::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}
*/

/*void ACIS_PlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}*/

void ACIS_PlayerState::OnRep_MySquadID()
{
	//@TODO: Let the squad subsystem know (once that exists)
}

void ACIS_PlayerState::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void ACIS_PlayerState::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 ACIS_PlayerState::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool ACIS_PlayerState::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

void ACIS_PlayerState::ClientBroadcastMessage_Implementation(const FCIS_VerbMessage Message)
{
	// This check is needed to prevent running the action when in standalone mode
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

