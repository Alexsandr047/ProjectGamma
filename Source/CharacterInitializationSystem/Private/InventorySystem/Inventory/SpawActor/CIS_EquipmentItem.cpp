// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Inventory/SpawActor/CIS_EquipmentItem.h"

#include "Character/CIS_Character.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACIS_EquipmentItem::ACIS_EquipmentItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	/*Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Item Skeletal Mesh"));
	SkeletalMeshComponent->bVisibleInReflectionCaptures = true;
	SkeletalMeshComponent->bCastHiddenShadow = true;
	SkeletalMeshComponent->SetTickGroup(TG_PostUpdateWork);
	SkeletalMeshComponent->SetupAttachment(Root);
	SkeletalMeshComponent->SetCollisionProfileName("No Collision");
	SkeletalMeshComponent->SetVisibility(true, true);
	SkeletalMeshComponent->AddTickPrerequisiteComponent(Root);*/
	
}

void ACIS_EquipmentItem::SetCharacterParam(ACIS_Character* Character)
{
	OwningCharacter = Character;
	if(OwningCharacter)
	{
		SetOwner(Character);
		OnRep_OwningCharacter();		
	}
}

void ACIS_EquipmentItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, OwningCharacter);
	DOREPLIFETIME(ThisClass, AttachmentElements);
	DOREPLIFETIME(ThisClass, SocketOnCharacter);
}

void ACIS_EquipmentItem::SetSocketOnCharacter(FName Socket)
{
	SocketOnCharacter = Socket;
}

void ACIS_EquipmentItem::SetAttachmentItems(TArray<TSubclassOf<ACIS_AttachmentItem>> AttachmentItems)
{
	AttachmentElements = AttachmentItems;
	InitAttachableElement();
}

void ACIS_EquipmentItem::InitAttachableElement()
{
	
}

void ACIS_EquipmentItem::OnRep_OwningCharacter()
{
	
}

void ACIS_EquipmentItem::OnRep_AttachableElements()
{
	
}

