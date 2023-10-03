// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CIS_AbilitySet.h"
#include "AbilitySystem/CIS_AbilitySystemComponent.h"
#include "Components/PawnComponent.h"
#include "CIS_ItemManagerComponent.generated.h"



class UCIS_EquipmentInstance;
class UCIS_ItemDefinition;
class UCIS_ItemManagerComponent;
class UCIS_ItemInstance;
struct FCIS_ItemList;


/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FCIS_ItemEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCIS_ItemEntry()
	{}

	FString GetDebugString() const;

private:
	friend FCIS_ItemList;
	friend UCIS_ItemManagerComponent;	

	UPROPERTY()
	UCIS_ItemInstance* Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FCIS_ItemList : public FFastArraySerializer
{
	GENERATED_BODY()

	FCIS_ItemList()
		: OwnerComponent(nullptr)
	{
	}

	FCIS_ItemList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UCIS_ItemInstance*> GetAllItems() const;
	
public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FCIS_ItemEntry, FCIS_ItemList>(Entries, DeltaParms, *this);
	}
		
	UCIS_ItemInstance* AddEntry(UCIS_ItemDefinition*& ItemClass, int32 StackCount);
	void AddEntry(UCIS_ItemInstance* Instance);

	void RemoveEntry(UCIS_ItemInstance* Instance);
private:
	friend UCIS_ItemManagerComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FCIS_ItemEntry> Entries;

	UPROPERTY()
	UActorComponent* OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FCIS_ItemList> : public TStructOpsTypeTraitsBase2<FCIS_ItemList>
{
	enum { WithNetDeltaSerializer = true };
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_ItemManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UCIS_ItemManagerComponent (const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UCIS_ItemInstance* AddItemDefinition(UCIS_ItemDefinition* ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UCIS_ItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UCIS_ItemDefinition> ItemDef) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UCIS_ItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UCIS_ItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveAllItemFromInventory();
	
	bool ConsumeItemsByDefinition(TSubclassOf<UCIS_ItemDefinition> ItemDef, int32 NumToConsume);


	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface
private:
	UPROPERTY(Replicated)
	FCIS_ItemList ItemList;
};
