// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/CIS_AbilitySet.h"
#include "GameFramework/Actor.h"
#include "CIS_ActorWithAbilities.generated.h"

class UCIS_AbilitySystemComponent;
class UCIS_AbilitySet;

UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_ActorWithAbilities : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACIS_ActorWithAbilities(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** AbilitySet this actor should be granted on spawn, if any */
	UPROPERTY(EditDefaultsOnly, Category="XCL|Ability")
	TObjectPtr<UCIS_AbilitySet> AbilitySet;
	
protected:
	//~AActor implementation
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor implementation

	/**
	 * This gets called when it's time to initialize the actor's ASC
	 *
	 * Override this as needed.  In your overrides, you MUST call Super()
	 * **BEFORE** you try to use the ASC at all.  The ASC is not functional
	 * before you call Super().
	 */
	virtual void InitializeAbilitySystem();

	/**
	 * This gets called when it's time to uninitialize the actor's ASC
	 */
	virtual void UninitializeAbilitySystem();

public:
	//~IAbilitySystemComponent interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UCIS_AbilitySystemComponent* GetCISAbilitySystemComponentChecked() const;
	//~End IAbilitySystemComponent interface

	
private:
	
	// The ability system component for this actor
	UPROPERTY(EditDefaultsOnly, Category="CIS|Ability")
	TObjectPtr<UCIS_AbilitySystemComponent> AbilitySystemComponent;

	// List of handles granted to this actor on spawn; these get cleaned up in EndPlay
	UPROPERTY(VisibleInstanceOnly, Category="CIS|Ability")
	FCIS_AbilitySet_GrantedHandles GrantedHandlesOnSpawn;
};
