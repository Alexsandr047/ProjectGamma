// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PG_PhysicalMaterialWithTags.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_PhysicalMaterialWithTags : public UPhysicalMaterial
{
	GENERATED_BODY()
public:
	UPG_PhysicalMaterialWithTags(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// A container of gameplay tags that game code can use to reason about this physical material
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=PhysicalProperties)
	FGameplayTagContainer Tags;
};
