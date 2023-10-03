// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InventorySystem/CIS_Interactable.h"
#include "GameFramework/Actor.h"
#include "Items/CIS_ActorWithAbilities.h"
#include "CIS_PickUpActor.generated.h"

USTRUCT(BlueprintType)
struct FPickUpParam
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionDuration = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target = nullptr;
};


UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_PickUpActor : public ACIS_ActorWithAbilities, public ICIS_Interactable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACIS_PickUpActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
