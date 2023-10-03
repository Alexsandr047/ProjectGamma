// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CIS_ActorWithAbilities.h"
#include "Weapons/PG_GameplayAbility_FromEquipment.h"

// Sets default values
ACIS_ActorWithAbilities::ACIS_ActorWithAbilities(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Must replicate Actor for multiplayer
	bReplicates = true;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UCIS_AbilitySystemComponent>(this, TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void ACIS_ActorWithAbilities::PostInitializeComponents()
{
	// BEFORE PostInit Components:
	InitializeAbilitySystem();

	// Now during PostInit Components, there is a functional ASC for other components to use
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void ACIS_ActorWithAbilities::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && AbilitySet)
	{
		UE_LOG(LogTemp, Warning, TEXT("Granting OnSpawn Ability Set [%s]"), *GetNameSafe(AbilitySet));

		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandlesOnSpawn);
	}
}

void ACIS_ActorWithAbilities::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeAbilitySystem();
	
	Super::EndPlay(EndPlayReason);
}

void ACIS_ActorWithAbilities::InitializeAbilitySystem()
{
	UE_LOG(LogTemp, Warning, TEXT("Initializing Ability System"));
	
	// We expect this to have been set in the constructor
	check(IsValid(AbilitySystemComponent));

	// Initialize ASC on this Actor
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACIS_ActorWithAbilities::UninitializeAbilitySystem()
{
	UE_LOG(LogTemp, Warning,TEXT("Uninitializing Ability System"));
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities();
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();
		AbilitySystemComponent->ClearActorInfo();
		
		if(AbilitySet)
			AbilitySet->GiveToAbilitySystem(GetCISAbilitySystemComponentChecked(), nullptr);
	}
}

UAbilitySystemComponent* ACIS_ActorWithAbilities::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCIS_AbilitySystemComponent* ACIS_ActorWithAbilities::GetCISAbilitySystemComponentChecked() const
{
	check(AbilitySystemComponent);
	return AbilitySystemComponent;
}
