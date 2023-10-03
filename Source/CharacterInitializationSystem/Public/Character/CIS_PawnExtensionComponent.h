// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CIS_PawnComponent.h"
#include "CIS_PawnExtensionComponent.generated.h"

/**
 * To use this system, you must use the ModularGameplay plugin, GameplayAbilities plugin.
 */

class UCIS_AbilitySystemComponent;
class AbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCIS_DynamicMulticastDelegate);

/**
 * UCIS_PawnExtensionComponent
 *
 * Component used to add functionality to all Pawn classes.
 */

UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_PawnExtensionComponent : public UCIS_PawnComponent
{
	GENERATED_BODY()

public:

	UCIS_PawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	// Возвращает extension component пишки если компонет существет в указанном акторе
	// Returns the pawn extension component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "CharacterInitializationSystem|Pawn")
	static UCIS_PawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCIS_PawnExtensionComponent>() : nullptr); }

	
	// Эту функцию необходимо вызывать  каждый карз конда нужно проверить готова ли пешка к инициализвации 
	// Call this anytime the pawn needs to check if it's ready to be initialized (pawn data assigned, possessed, etc..). 
	bool CheckPawnReadyToInitialize();

	// Зарегистрировасть с делегатом OnPawnReadyToInitialize и broadcast если состояние удовлетворяет требованиям. 
	// Register with the OnPawnReadyToInitialize delegate and broadcast if condition is already met.
	void OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

	// Должен быть вызван у используемой пешки когда компонет ввода настроен.
	// Should be called by the owning pawn when the input component is setup.
	void SetupPlayerInputComponent();

	void PrepareForCheckPawnReadyToInitialize();
	// Should be called by the owning pawn when the pawn's controller changes.
	void HandleControllerChanged();

	// Should be called by the owning pawn when the player state has been replicated.
	void HandlePlayerStateReplicated();
	
	UFUNCTION(BlueprintPure, Category = "Liberty|Pawn")
	UCIS_AbilitySystemComponent* GetLiberty_AbilitySystemComponent() const { return AbilitySystemComponent; }

	// Should be called by the owning pawn to become the avatar of the ability system.
	void InitializeAbilitySystem(UCIS_AbilitySystemComponent* InASC, AActor* InOwnerActor);

	// Should be called by the owning pawn to remove itself as the avatar of the ability system.
	void UninitializeAbilitySystem();
	
	// Register with the OnAbilitySystemInitialized delegate and broadcast if condition is already met.
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

	// Register with the OnAbilitySystemUninitialized delegate.
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

	
protected:

	// Delegate fired when pawn has everything needed for initialization.
	FSimpleMulticastDelegate OnPawnReadyToInitialize;

	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "On Pawn Ready To Initialize"))
	FCIS_DynamicMulticastDelegate BP_OnPawnReadyToInitialize;
	
	// Delegate fired when our pawn becomes the ability system's avatar actor
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	// Delegate fired when our pawn is removed as the ability system's avatar actor
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
	
protected:

	// Pointer to the ability system component that is cached for convenience.
	UPROPERTY()
	UCIS_AbilitySystemComponent* AbilitySystemComponent;
	
	// True when the pawn has everything needed for initialization.
	int32 bPawnReadyToInitialize : 1;
	
};