// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CIS_Character.h"
#include "UObject/NoExportTypes.h"
#include "CIS_EquipmentInstance.generated.h"


class UCIS_OutfitInstance;
class UCIS_WeaponInstance;
class UCIS_AttachableElement;
class ACIS_AttachmentItem;
class ACIS_EquipmentItem;


/**
 * UCIS_EquipmentInstance
 *
 * A piece of Item spawned and applied to a pawn
 */
UCLASS(BlueprintType, Blueprintable)
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_EquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	
	UCIS_EquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface


	UFUNCTION(BlueprintPure, Category=Equipment)
	UObject* GetInstigator() const { return Instigator; }

	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category=Equipment, meta=(DeterminesOutputType=PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	virtual void SpawnItemActors(TSubclassOf<ACIS_EquipmentItem> ActorsToSpawn, FName SocketName,TArray<TSubclassOf<ACIS_AttachmentItem>> AttachmentItems);
	virtual void DestroyItemActors();
	
	virtual void OnEquipped();
	virtual void OnUnequipped();

	

	UFUNCTION(BlueprintPure, Category=Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }
	
/*protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category=Equipment, meta=(DisplayName="OnUnequipped"))
	void K2_OnUnequipped();*/

private:
	UFUNCTION()
	void OnRep_Instigator();


private:

	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	UObject* Instigator;
	
	UPROPERTY(Replicated)
	TArray<AActor*> SpawnedActors;

};
