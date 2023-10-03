// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CIS_AbilitySet.h"
#include "UObject/NoExportTypes.h"
#include "CIS_EquipmentDefinition.generated.h"

class UCIS_EquipmentInstance;
class UCIS_AttachmentLogic;
class ACIS_EquipmentItem;
class ACIS_AttachmentItem;
class UCIS_ItemDefinition;



UCLASS(BlueprintType, Blueprintable, Abstract)
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_AttachmentLogic : public UObject
{
	GENERATED_BODY()

public:
	
	UCIS_AttachmentLogic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent)
	bool CheckForUseOfAttachment(const TArray< UCIS_ItemDefinition*>& AttachmentDefinition, UCIS_ItemDefinition* CheckedAttachmentDefinition);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	TArray<FGameplayTag> GetAttachmentSlots(const TArray<UCIS_ItemDefinition*>& AttachmentDefinition);
};


/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_EquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:

	UCIS_EquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UCIS_EquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<const UCIS_AbilitySet>> AbilitySetsToGrant;

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<ACIS_EquipmentItem> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	TArray<TSubclassOf<ACIS_AttachmentItem>> AttachmentItems;
	
	void SetParams(TSubclassOf<UCIS_EquipmentInstance> _InstanceType, TArray<TObjectPtr<const UCIS_AbilitySet>> _AbilitySetsToGrant,
	TSubclassOf<ACIS_EquipmentItem> _ActorToSpawn, TArray<TSubclassOf<ACIS_AttachmentItem>> _AttachmentItems);
	
};
