// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "AbilitySystem/CIS_AbilitySystemComponent.h"
#include "Core/CIS_GameplayTagStack.h"
#include "GameFramework/PlayerState.h"
#include "Messages/CIS_VerbMessage.h"
#include "CIS_PlayerState.generated.h"

class UCIS_AbilitySystemComponent;


/** Defines the types of client connected */
UENUM()
enum class ECIS_PlayerConnectionType : uint8
{
	// An active player
	Player = 0,

	// Spectator connected to a running game
	LiveSpectator,

	// Spectating a demo recording offline
	ReplaySpectator,

	// A deactivated player (disconnected)
	InactivePlayer
};


UCLASS(Config = Game)
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_PlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
		
public:

	ACIS_PlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "CIS|PlayerState")
	ACIS_PlayerController* GetCIS_PlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "CIS|PlayerState")
	UCIS_AbilitySystemComponent* GetCIS_AbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	
	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;
	//~End of APlayerState interface

	//~ILyraTeamAgentInterface interface
	/*virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnLyraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;*/
	//~End of ILyraTeamAgentInterface interface

	
	static const FName NAME_LyraAbilityReady;

	void SetPlayerConnectionType(ECIS_PlayerConnectionType NewType);
	ECIS_PlayerConnectionType GetPlayerConnectionType() const { return MyPlayerConnectionType; }

	/** Returns the Squad ID of the squad the player belongs to. */
	/*UFUNCTION(BlueprintCallable)
	int32 GetSquadId() const
	{
		return MySquadID;
	}*/

	/** Returns the Team ID of the team the player belongs to. */
	/*UFUNCTION(BlueprintCallable)
	int32 GetTeamId() const
	{
		return GenericTeamIdToInteger(MyTeamID);
	}*/
	
	void SetSquadID(int32 NewSquadID);

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Teams)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Teams)
	bool HasStatTag(FGameplayTag Tag) const;

	// Send a message to just this player
	// (use only for client notifications like accolades, quest toasts, etc... that can handle being occasionally lost)
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "Lyra|PlayerState")
	void ClientBroadcastMessage(const FCIS_VerbMessage Message);

private:
	//void OnExperienceLoaded(const UCIS_ExperienceDefinition* CurrentExperience);

protected:
	/*UFUNCTION()
	void OnRep_PawnData();*/

protected:

	/*UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const ULyraPawnData> PawnData;*/

private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "CIS|PlayerState")
	TObjectPtr<UCIS_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Replicated)
	ECIS_PlayerConnectionType MyPlayerConnectionType;

	/*UPROPERTY()
	FOnLyraTeamIndexChangedDelegate OnTeamChangedDelegate;*/

	/*UPROPERTY(ReplicatedUsing=OnRep_MyTeamID)
	FGenericTeamId MyTeamID;*/

	UPROPERTY(ReplicatedUsing=OnRep_MySquadID)
	int32 MySquadID;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

private:
	/*
	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);
	*/

	UFUNCTION()
	void OnRep_MySquadID();
};
