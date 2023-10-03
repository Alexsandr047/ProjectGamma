// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PG_CharacterPresets_Component.h"

#include "Core/PG_AssetManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPG_CharacterPresets_Component::UPG_CharacterPresets_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPG_CharacterPresets_Component::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, UpdateCharacterPreset);
}

void UPG_CharacterPresets_Component::InitCharacterPreset()
{
	
	UpdateCharacterPreset = UPG_AssetManager::Get().GetCharacterPresets()->CharacterPresets;
	OnRep_UpdateCharacterPreset();
}

void UPG_CharacterPresets_Component::OnRep_UpdateCharacterPreset()
{
	if(ACIS_Character* Character = Cast<ACIS_Character>(GetOwner()))
	{
		if(USkeletalMeshComponent* MeshComp = Character->GetMesh())
		{
			MeshComp->SetSkeletalMesh(UpdateCharacterPreset.UpdateSkeletalMesh);
			MeshComp->SetAnimClass(UpdateCharacterPreset.UpdateAnimInstanceClass);
			MeshComp->LinkAnimClassLayers(UpdateCharacterPreset.UpdateAnimLayersInstanceClass);
		}
	}
}



// Called when the game starts
void UPG_CharacterPresets_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

