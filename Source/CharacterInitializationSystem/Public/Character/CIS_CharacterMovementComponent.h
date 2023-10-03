// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CIS_CharacterMovementComponent.generated.h"

CHARACTERINITIALIZATIONSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_MovementStopped);


/**
 * FCIS_CharacterGroundInfo
 *
 *	Information about the ground under the character.  It only gets updated as needed.
 */


USTRUCT(BlueprintType)
struct FCIS_CharacterGroundInfo
{
	GENERATED_BODY()

	FCIS_CharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

/**
 * UCIS_MovementComponent
 *
 *	The base character movement component class used by this project.
 */

UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UCIS_CharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void SimulateMovement(float DeltaTime) override;

	virtual bool CanAttemptJump() const override;
	
	// Returns the current ground info.  Calling this will update the ground info if it's out of date.
	UFUNCTION(BlueprintCallable, Category = "Lyra|CharacterMovement")
	const FCIS_CharacterGroundInfo& GetGroundInfo();

	void SetReplicatedAcceleration(const FVector& InAcceleration);

	//~UMovementComponent interface
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual float GetMaxSpeed() const override;
	//~End of UMovementComponent interface

	/** If true, try to sprint (or keep sprint) on next update. If false, try to stop sprinting on next update. */
	UPROPERTY(Category="Character Movement (General Settings)", VisibleInstanceOnly, BlueprintReadOnly)
	uint8 RequestToStartSprinting:1;

	// Sprint
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprinting();
	
	virtual void InitializeComponent() override;

protected:

	// Cached ground info for the character.  Do not access this directly!  It's only updated when accessed via GetGroundInfo().
	FCIS_CharacterGroundInfo CachedGroundInfo;

	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;
};
