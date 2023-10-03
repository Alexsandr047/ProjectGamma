// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/SpawnActor/PG_WeaponItem.h"

#include "Character/CIS_Character.h"
#include "InventorySystem/SpawnActor/PG_WeaponAttachmentItem.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/PG_WeaponFire.h"


APG_WeaponItem::APG_WeaponItem()
{

	bReplicates = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Item Skeletal Mesh"));
	SkeletalMeshComponent->bVisibleInReflectionCaptures = true;
	SkeletalMeshComponent->bCastHiddenShadow = true;
	SkeletalMeshComponent->SetTickGroup(TG_PostUpdateWork);
	SkeletalMeshComponent->SetupAttachment(Root);
	SkeletalMeshComponent->SetCollisionProfileName("No Collision");
	SkeletalMeshComponent->SetVisibility(true, true);
	SkeletalMeshComponent->AddTickPrerequisiteComponent(Root);

	
}
void APG_WeaponItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, SkeletalMeshComponent);
}
void APG_WeaponItem::InitAttachableElement()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		for(auto& Attachment: AttachmentElements)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APG_WeaponAttachmentItem* WeaponAttachmentItem;
			WeaponAttachmentItem = GetWorld()->SpawnActor<APG_WeaponAttachmentItem>(Attachment, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			if(WeaponAttachmentItem)
			{
				WeaponAttachmentItem->AttachElement(this);
				WeaponAttachmentItem->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachmentItem->SocketName);
				AttachmentActors.Add(WeaponAttachmentItem);
			}
		}
	}
}

void APG_WeaponItem::OnRep_OwningCharacter()
{
	if(OwningCharacter && SkeletalMeshComponent)
	{
		USceneComponent* AttachTarget = OwningCharacter->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningCharacter))
		{
			AttachTarget = Char->GetMesh();
		}
		AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SocketOnCharacter);
	}
}

void APG_WeaponItem::Fire(TArray<FVector> ImpactPositions, FVector ImpactNormals, EPhysicalSurface ImpactSurfaceTypes)
{

	if(bNeedsFakeProjectileDat)
	{
		AddFakeProjectileData(NumberOfFakeProjectiles);
	}

	//Spawn and activate Weapon Fire effects. We need seperate actors so as to not destroy the effects when switching to another weapon. The actors containing the effects gets destroyed when all systems are dead
	if(!WeaponFire)
	{
		WeaponFire = GetWorld()->SpawnActorDeferred<APG_WeaponFire>(WeaponFireActor, FTransform::Identity, nullptr);
		WeaponFire->AttachToComponent(SkeletalMeshComponent,  FAttachmentTransformRules::KeepRelativeTransform,"");
		WeaponFire->Init(SkeletalMeshComponent, EnableShellEject, ShellEjectSystem, EnableMuzzleFlash, MuzzleFlashSystem, EnableTracer, TracerSystem, ShellEjectMesh);
	}
	if(WeaponFire)
	{
		WeaponFire->Fire(ImpactPositions, ImpactNormals, ImpactSurfaceTypes);
	}
}

void APG_WeaponItem::AddFakeProjectileData(int32 _NumberOfFakeProjectiles, float ConeHalfAngleInDegrees)
{
	
}
