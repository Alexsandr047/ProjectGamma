#include "InventorySystem/Equipment/CIS_EquipmentManagerComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/CIS_AbilitySystemComponent.h"
#include "Engine/ActorChannel.h"
#include "InventorySystem/Equipment/CIS_EquipmentDefinition.h"
#include "InventorySystem/Equipment/CIS_EquipmentInstance.h"
#include "Net/UnrealNetwork.h"


FString FCIS_AppliedEquipmentEntry::GetDebugString() const
{
	return  "";
}

void FCIS_EquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FCIS_AppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

void FCIS_EquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	/*for (int32 Index : AddedIndices)
	{
		const FCIS_AppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}*/
}

void FCIS_EquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	
}

UCIS_EquipmentInstance* FCIS_EquipmentList::AddEntry(UCIS_EquipmentDefinition* EquipmentDefinition, FName SocketName)
{
	UCIS_EquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	TSubclassOf<UCIS_EquipmentInstance> InstanceType = EquipmentDefinition->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UCIS_EquipmentInstance::StaticClass();
	}
	
	FCIS_AppliedEquipmentEntry& NewEntry =  Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UCIS_EquipmentInstance>(OwnerComponent->GetOwner(), InstanceType); //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;	
	
	if (UCIS_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (TObjectPtr<const UCIS_AbilitySet> AbilitySet : EquipmentDefinition->AbilitySetsToGrant)
		{
			if(AbilitySet)
				AbilitySet->GiveToAbilitySystem(ASC,  /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?	
	}
	
	Result->SpawnItemActors(EquipmentDefinition->ActorToSpawn, SocketName, EquipmentDefinition->AttachmentItems);
	
	MarkItemDirty(NewEntry);

	return Result;
}

void FCIS_EquipmentList::RemoveEntry(UCIS_EquipmentInstance* Instance)
{	
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FCIS_AppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UCIS_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyItemActors();			

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UCIS_AbilitySystemComponent* FCIS_EquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UCIS_AbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UCIS_EquipmentManagerComponent::UCIS_EquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UCIS_EquipmentManagerComponent::UnequipItem(UCIS_EquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}	
}

UCIS_EquipmentInstance* UCIS_EquipmentManagerComponent::EquipItem(UCIS_EquipmentDefinition* EquipmentClass,
	FName SocketName)
{
	UCIS_EquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass, SocketName);
		/*if (Result != nullptr)
		{
			Result->OnEquipped();
		}*/
	}
	return Result;
}

bool UCIS_EquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
                                                         FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FCIS_AppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UCIS_EquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UCIS_EquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UCIS_EquipmentManagerComponent::UninitializeComponent()
{
	TArray<UCIS_EquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FCIS_AppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UCIS_EquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		/*UnequipItem(EquipInstance);*/
	}

	Super::UninitializeComponent();
}

UCIS_EquipmentInstance* UCIS_EquipmentManagerComponent::GetFirstInstanceOfType(
	TSubclassOf<UCIS_EquipmentInstance> InstanceType)
{
	for (FCIS_AppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UCIS_EquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UCIS_EquipmentInstance*> UCIS_EquipmentManagerComponent::GetEquipmentInstancesOfType(
	TSubclassOf<UCIS_EquipmentInstance> InstanceType) const
{
	TArray<UCIS_EquipmentInstance*> Results;
	for (const FCIS_AppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UCIS_EquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}
void UCIS_EquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}
