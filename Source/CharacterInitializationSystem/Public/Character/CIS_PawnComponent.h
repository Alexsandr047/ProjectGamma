// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "CIS_PawnComponent.generated.h"


/**
 * To use this system, you must use the ModularGameplay plugin, GameplayAbilities plugin.
 */

UINTERFACE(BlueprintType)
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_ReadyInterface : public UInterface
{
	GENERATED_BODY()
};

class ICIS_ReadyInterface
{
	GENERATED_BODY()

public:
	virtual bool IsPawnComponentReadyToInitialize()  const = 0;
};

/**
 * UCIS_PawnComponent
 *
 * An actor component that can be used for adding custom behavior to pawns.
 */

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_PawnComponent : public UPawnComponent, public ICIS_ReadyInterface
{
	GENERATED_BODY()
	
public:
	
	UCIS_PawnComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual bool IsPawnComponentReadyToInitialize() const override { return true; };
};