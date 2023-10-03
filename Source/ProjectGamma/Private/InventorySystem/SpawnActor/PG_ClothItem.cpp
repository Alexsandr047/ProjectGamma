// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/SpawnActor/PG_ClothItem.h"

#include "Character/CIS_Character.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

APG_ClothItem::APG_ClothItem()
{
	bReplicates = true;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Item Skeletal Mesh"));
	SkeletalMeshComponent->bVisibleInReflectionCaptures = true;
	SkeletalMeshComponent->bCastHiddenShadow = true;
	SkeletalMeshComponent->SetTickGroup(TG_PostUpdateWork);
	SkeletalMeshComponent->SetCollisionProfileName("No Collision");
	SkeletalMeshComponent->SetVisibility(true, true);
	
	RootComponent = SkeletalMeshComponent;
}

void APG_ClothItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, SkeletalMeshComponent);
}

void APG_ClothItem::OnRep_OwningCharacter()
{
	if(OwningCharacter && SkeletalMeshComponent)
	{
		
		SkeletalMeshComponent->SetLeaderPoseComponent(OwningCharacter->GetMesh());
		SkeletalMeshComponent->SetRelativeTransform(OwningCharacter->GetMesh()->GetRelativeTransform());
	}
}
