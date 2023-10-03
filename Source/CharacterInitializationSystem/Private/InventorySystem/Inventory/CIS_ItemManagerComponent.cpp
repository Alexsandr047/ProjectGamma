// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory/CIS_ItemManagerComponent.h"
#include "AbilitySystem/CIS_AbilitySystemComponent.h"
#include "Engine/ActorChannel.h"
#include "InventorySystem/Equipment/CIS_EquipmentInstance.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"
#include "InventorySystem/Inventory/CIS_ItemInstance.h"
#include "Net/UnrealNetwork.h"


FString FCIS_ItemEntry::GetDebugString() const
{
	UCIS_ItemDefinition ItemDef;
	if (Instance != nullptr)
	{
		//ItemDef = Instance->GetItemDef();
	}

	//return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));

	return "nullptr";
}

TArray<UCIS_ItemInstance*> FCIS_ItemList::GetAllItems() const
{
	TArray<UCIS_ItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FCIS_ItemEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
			{
			Results.Add(Entry.Instance);
			}
	}
	return Results;
}

void FCIS_ItemList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FCIS_ItemEntry& Stack = Entries[Index];
		//BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FCIS_ItemList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FCIS_ItemEntry& Stack = Entries[Index];
		//BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FCIS_ItemList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FCIS_ItemEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		//BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

UCIS_ItemInstance* FCIS_ItemList::AddEntry(UCIS_ItemDefinition*& ItemDef, int32 StackCount)
{
	UCIS_ItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FCIS_ItemEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UCIS_ItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);	
	for (UCIS_ItemFragment* Fragment : ItemDef->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	NewEntry.Instance->InitAttachable(ItemDef->Fragments, NewEntry.Instance);
	
	NewEntry.StackCount = StackCount;
	
	Result = NewEntry.Instance;
	
	MarkItemDirty(NewEntry);

	return Result;
}

void FCIS_ItemList::AddEntry(UCIS_ItemInstance* Instance)
{
	unimplemented();
}

void FCIS_ItemList::RemoveEntry(UCIS_ItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FCIS_ItemEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UCIS_ItemManagerComponent::UCIS_ItemManagerComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
, ItemList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

UCIS_ItemInstance* UCIS_ItemManagerComponent::AddItemDefinition(UCIS_ItemDefinition* ItemDef, int32 StackCount)
{
	UCIS_ItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = ItemList.AddEntry(ItemDef, StackCount);
	}
	return Result;
}

UCIS_ItemInstance* UCIS_ItemManagerComponent::FindFirstItemStackByDefinition(
	TSubclassOf<UCIS_ItemDefinition> ItemDef) const
{
	for (const FCIS_ItemEntry& Entry : ItemList.Entries)
	{
		UCIS_ItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			/*if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}*/
		}
	}

	return nullptr;
}

void UCIS_ItemManagerComponent::RemoveItemInstance(UCIS_ItemInstance* ItemInstance)
{
	ItemList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UCIS_ItemInstance*> UCIS_ItemManagerComponent::GetAllItems() const
{
	return ItemList.GetAllItems();
}

void UCIS_ItemManagerComponent::RemoveAllItemFromInventory()
{
	for(auto& Item: GetAllItems())
	{
		RemoveItemInstance(Item);
	}
}

void UCIS_ItemManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemList);
}

bool UCIS_ItemManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UCIS_ItemDefinition> ItemDef,
	int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UCIS_ItemInstance* Instance = UCIS_ItemManagerComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			ItemList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

bool UCIS_ItemManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FCIS_ItemEntry& Entry : ItemList.Entries)
	{
		UCIS_ItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{			
			if(Instance->GetItemDef())
			{
				WroteSomething |= Channel->ReplicateSubobject(Instance->GetItemDef(), *Bunch, *RepFlags);
			}
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}