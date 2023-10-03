// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/SpawnActor/PG_WeaponAttachmentItem.h"

APG_WeaponAttachmentItem::APG_WeaponAttachmentItem()
{
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;
	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AttachableMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Weapon Attachment Mesh"));
	AttachableMesh->SetupAttachment(Root);
	AttachableMesh->SetCollisionProfileName("No Collision");
	AttachableMesh->SetVisibility(true, true);
	SocketName = "";
}

void APG_WeaponAttachmentItem::AttachElement(APG_WeaponItem* Weapon)
{
	if(!Weapon)
		return;
	
	AttachableMesh->AttachToComponent(Weapon->SkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	AttachableMesh->bVisibleInReflectionCaptures = true;
	AttachableMesh->bCastHiddenShadow = true;
	AttachableMesh->SetTickGroup(TG_PostUpdateWork);
}
