// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PG_AttachWeaponToSocketAnimNotify.h"

#include "Character/PG_Character.h"
#include "InventorySystem/Equipment/CIS_EquipmentManagerComponent.h"
#include "Weapons/PG_WeaponInstance.h"

UPG_AttachWeaponToSocketAnimNotify::UPG_AttachWeaponToSocketAnimNotify()
{
	SocketName = "";
}

void UPG_AttachWeaponToSocketAnimNotify::AttachWeaponToSocket(USkeletalMeshComponent* MeshComp)
{
	APG_Character* Character = Cast<APG_Character>(MeshComp->GetOwner());
	if(!Character)
		return;

	UCIS_EquipmentManagerComponent* EMC = Cast<UCIS_EquipmentManagerComponent>(Character->GetComponentByClass(UCIS_EquipmentManagerComponent::StaticClass()));
	if(!EMC)
		return;

	TArray<UCIS_EquipmentInstance*> EI = EMC->GetEquipmentInstancesOfType(UPG_WeaponInstance::StaticClass());

	for(auto& EquipInst : EI)
	{
		if(UPG_WeaponInstance* WI = Cast<UPG_WeaponInstance>(EquipInst))
		{
			TArray<AActor*> Actors = WI->GetSpawnedActors();
			if(WI->GetIsCurrentWeapon())
			{
				if(Actors.Num() > 0)
				{
					USceneComponent* AttachTarget = Character->GetRootComponent();
					if (ACharacter* Char = Cast<ACharacter>(Character))
					{
						AttachTarget = Char->GetMesh();
					}
					AActor* Weapon = Cast<AActor>(Actors[0]);
					Weapon->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
				}
			}
		}
	}
}
