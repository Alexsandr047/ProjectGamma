// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/CIS_AbilitySet.h"
#include "Camera/CIS_CameraComponent.h"
#include "GameFramework/Character.h"
#include "CIS_Character.generated.h"

class UCIS_PawnExtensionComponent;
class UCIS_AbilitySystemComponent;
class ACIS_PlayerState;
class UCIS_HealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwapCameraDelegate, bool, bFirstPesron);

/**
 * FCIS_ReplicatedAcceleration: Compressed representation of acceleration
 */
USTRUCT()
struct FCIS_ReplicatedAcceleration
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// Direction of XY accel component, quantized to represent [0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	//Accel rate of XY component, quantized to represent [0, MaxAcceleration]

	UPROPERTY()
	int8 AccelZ = 0;	// Raw Z accel rate component, quantized to represent [-MaxAcceleration, MaxAcceleration]
};


UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACIS_Character(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void InitCharacterData();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterInitializationSystem|Character", Meta = (AllowPrivateAccess = "true"))
	UCIS_HealthComponent* HealthComponent;

#pragma region AActor_Interface
	
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	UFUNCTION(BlueprintCallable, Category = "Liberty|Character")
	ACIS_PlayerState* GetCIS_PlayerState() const;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;


	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	void InitializeGameplayTags();

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled);


	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	virtual void UninitAndDestroy();

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();


	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	
	virtual bool CanJumpInternal_Implementation() const;

	
private:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReplicatedAcceleration)
	FCIS_ReplicatedAcceleration ReplicatedAcceleration;
	
	UFUNCTION()
	void OnRep_ReplicatedAcceleration();
#pragma endregion
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "CharacterInitializationSystem|Character")
	UCIS_AbilitySystemComponent* GetCIS_AbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterInitializationSystem|Abilities")
	UCIS_AbilitySet* AbilitySet;
	
protected:
	
	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void ToggleCrouch();

#pragma region VisibilyPart

private:	
	UPROPERTY()
	USkeletalMesh* UpdateSkeletalMesh;
	UPROPERTY()
	TSubclassOf<UAnimInstance> UpdateAnimInstanceClass;
	
	
	// List of handles granted to this actor on spawn; these get cleaned up in EndPlay
	UPROPERTY(VisibleInstanceOnly, Category="CharacterInitializationSystem|Abilities")
	FCIS_AbilitySet_GrantedHandles GrantedHandlesOnSpawn;

#pragma endregion 
};
