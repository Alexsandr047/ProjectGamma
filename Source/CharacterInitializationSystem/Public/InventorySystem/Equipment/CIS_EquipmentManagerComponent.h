// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CIS_AbilitySet.h"
#include "Components/PawnComponent.h"
#include "CIS_EquipmentManagerComponent.generated.h"

struct FCIS_EquipmentList;
class UCIS_EquipmentManagerComponent;
class UCIS_EquipmentDefinition;
class UCIS_EquipmentInstance;
class UCIS_OutfitInstance;
class UCIS_ClothDefinition;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FCIS_AppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FCIS_AppliedEquipmentEntry()
	{
	}

	FString GetDebugString() const;

private:
	friend FCIS_EquipmentList;
	friend UCIS_EquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	UCIS_EquipmentDefinition* EquipmentDefinition;
	
	UPROPERTY()
	UCIS_EquipmentInstance* Instance = nullptr;
	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FCIS_AbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FCIS_EquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FCIS_EquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FCIS_EquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize(Entries, DeltaParms, *this);
	}

	UCIS_EquipmentInstance* AddEntry(UCIS_EquipmentDefinition* EquipmentDefinition, FName SocketName);
	void RemoveEntry(UCIS_EquipmentInstance* Instance);

private:
	UCIS_AbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UCIS_EquipmentManagerComponent;

public:
	
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FCIS_AppliedEquipmentEntry> Entries;
	
	UPROPERTY()
	UActorComponent* OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FCIS_EquipmentList> : public TStructOpsTypeTraitsBase2<FCIS_EquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

UCLASS(BlueprintType, Const)
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_EquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	UCIS_EquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UCIS_EquipmentInstance* EquipItem(UCIS_EquipmentDefinition* EquipmentClass, FName SocketName);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UCIS_EquipmentInstance* ItemInstance);

	
	/*
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	ULS_EquipmentInstance* EquipWeapon(ULS_EquipmentDefinition* EquipmentClass, FName SocketName);*/

	/*UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(ULS_EquipmentInstance* ItemInstance);*/
	
	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCIS_EquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UCIS_EquipmentInstance> InstanceType);
	
	/** Returns all equipped instances of a given type, or an empty array if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UCIS_EquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UCIS_EquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FCIS_EquipmentList EquipmentList;

};
