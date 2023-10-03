// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Equipment/CIS_EquipmentInstance.h"

#include "InventorySystem/Inventory/SpawActor/CIS_EquipmentItem.h"
#include "Net/UnrealNetwork.h"

UCIS_EquipmentInstance::UCIS_EquipmentInstance(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

UWorld* UCIS_EquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

APawn* UCIS_EquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UCIS_EquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}

void UCIS_EquipmentInstance::SpawnItemActors(TSubclassOf<ACIS_EquipmentItem> ActorsToSpawn, FName Socket, TArray<TSubclassOf<ACIS_AttachmentItem>> AttachmentItems)
{
	auto Pawn = GetPawn();
	if (APawn* OwningPawn = Pawn)
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		/*if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}*/

		if (ActorsToSpawn)			
		{			
			ACIS_EquipmentItem* NewActor = GetWorld()->SpawnActorDeferred<ACIS_EquipmentItem>(ActorsToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/true);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform,Socket);
			SpawnedActors.Add(NewActor);
			ACIS_Character* Character = Cast<ACIS_Character>(OwningPawn);
			if(Character)
			{
				NewActor->SetSocketOnCharacter(Socket);
				NewActor->SetCharacterParam(Character);
				NewActor->SetAttachmentItems(AttachmentItems);
				for(auto& Actor : NewActor->GetAttachmentActors())
				{
					SpawnedActors.Add(Actor);
				}
			}
		}
	}
}


void UCIS_EquipmentInstance::OnEquipped()
{
	//K2_OnEquipped();
}

void UCIS_EquipmentInstance::OnUnequipped()
{
	//K2_OnUnequipped();
}

void UCIS_EquipmentInstance::DestroyItemActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}


void UCIS_EquipmentInstance::OnRep_Instigator()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UCIS_EquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}
