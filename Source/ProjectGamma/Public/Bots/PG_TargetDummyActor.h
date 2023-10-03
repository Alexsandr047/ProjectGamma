// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/CIS_ActorWithAbilitiesAndHealth.h"
#include "PG_TargetDummyActor.generated.h"

/**
 * PG Target Dummy Actor
 *
 * This is an Immortal Actor with Abilities and Health.
 *
 * It will never go below 1 Health.
 */
UCLASS()
class PROJECTGAMMA_API APG_TargetDummyActor : public ACIS_ActorWithAbilitiesAndHealth
{
	GENERATED_BODY()

public:
	// Set Class Defaults
	APG_TargetDummyActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UXCLActorWithAbilities implementation
	virtual void InitializeAbilitySystem() override;
	//~End of UXCLActorWithAbilities implementation

	//~AActor implementation
	virtual void BeginPlay() override;
	//~End of AActor implementation

	/** Get the actor's root skeletal mesh component */
	UFUNCTION(BlueprintPure)
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return MeshComponent; }

protected:
	/** Root Component is Skeletal Mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
};
