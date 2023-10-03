// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PG_WeaponInstance.h"

#include "Character/PG_Character.h"
#include "Net/UnrealNetwork.h"

void UPG_WeaponInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsCurrentWeapon);
	DOREPLIFETIME(ThisClass, bWeaponPose);
	DOREPLIFETIME(ThisClass, bUnarmedPose);
}

void UPG_WeaponInstance::OnEquipped()
{
	bIsCurrentWeapon = true;
	bWeaponPose = !bWeaponPose;
	OnRep_UseWeaponPose();
	Super::OnEquipped();
}

void UPG_WeaponInstance::OnUnequipped()
{
	bIsCurrentWeapon = false;
	bUnarmedPose = !bUnarmedPose;
	OnRep_UseUnarmedPose();
	Super::OnUnequipped();
}

void UPG_WeaponInstance::SetIsCurrentWeapon(bool IsCurrentWeapon)
{
	bIsCurrentWeapon = IsCurrentWeapon;
}

void UPG_WeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	TimeLastFired = World->GetTimeSeconds();
}

void UPG_WeaponInstance::OnRep_UseWeaponPose()
{
	if(APG_Character* Pawn = Cast<APG_Character>(GetPawn()))
	{
		Pawn->GetMesh()->LinkAnimClassLayers(WeaponPose);
	}
	
}

void UPG_WeaponInstance::OnRep_UseUnarmedPose()
{
	if(APG_Character* Pawn = Cast<APG_Character>(GetPawn()))
	{
		Pawn->GetMesh()->LinkAnimClassLayers(WeaponPose);
	}
}

