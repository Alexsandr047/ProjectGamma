// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CIS_ItemDefinition.h"
#include "GameplayTagContainer.h"
#include "Core/CIS_CoreTypes.h"
#include "Core/CIS_GameplayTagStack.h"
#include "InventorySystem/Equipment/CIS_EquipmentDefinition.h"
#include "UObject/NoExportTypes.h"
#include "CIS_ItemInstance.generated.h"


USTRUCT(BlueprintType)
struct FAttachableParam
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemSlotTag;
	UPROPERTY(BlueprintReadOnly)
	FString ItemDescriptionId;
};


UCLASS(BlueprintType, Blueprintable)
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_ItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
	
	UCIS_ItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;
		
	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category= Inventory)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool HasStatTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable)
	UCIS_ItemDefinition* GetItemDef() const
	{
		return ItemDef;
	}

	UFUNCTION(BlueprintCallable)
	TArray<FAttachableParam> GetAttachableParam() const
	{
		return AttachableParam;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UCIS_ItemFragment* FindFragmentByClass(TSubclassOf<UCIS_ItemFragment> FragmentClass) const;
	
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	void SetItemDef(UCIS_ItemDefinition* ItemDef);
	void InitAttachable(TArray<UCIS_ItemFragment*> ItemFragments, UCIS_ItemInstance* Instance);
	void SetUpAttachmentFragment(UCIS_ItemDefinition* AttachmentDef);
	void RemoveAttachmentFragment(/*UCIS_ItemCategory* Category*/);
	
	UFUNCTION()
	void OnRep_ItemDefinition();

private:
	friend struct FLiberty_InventoryList;

private:
	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;
	
	UPROPERTY(ReplicatedUsing=OnRep_ItemDefinition)
	UCIS_ItemDefinition* ItemDef;

	UPROPERTY(Replicated)
	TArray<FAttachableParam> AttachableParam;
};
