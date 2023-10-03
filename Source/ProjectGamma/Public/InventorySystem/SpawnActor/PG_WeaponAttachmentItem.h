// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_WeaponItem.h"
#include "InventorySystem/Inventory/SpawActor/CIS_AttachmentItem.h"
#include "PG_WeaponAttachmentItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API APG_WeaponAttachmentItem : public ACIS_AttachmentItem
{
	GENERATED_BODY()

public:
	
	APG_WeaponAttachmentItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite/*, Replicated*/)
	UStaticMeshComponent* AttachableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

	void AttachElement(APG_WeaponItem* Weapon);
};
