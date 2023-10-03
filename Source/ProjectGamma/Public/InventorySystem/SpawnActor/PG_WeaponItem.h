// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Inventory/SpawActor/CIS_EquipmentItem.h"
#include "PG_WeaponItem.generated.h"

class APG_WeaponFire;
class UNiagaraSystem;

UCLASS()
class PROJECTGAMMA_API APG_WeaponItem : public ACIS_EquipmentItem
{
	GENERATED_BODY()
	
public:

	APG_WeaponItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	virtual void InitAttachableElement() override;

	virtual void OnRep_OwningCharacter() override;


	UFUNCTION(BlueprintCallable)
	void Fire(TArray<FVector> ImpactPositions, FVector ImpactNormals,EPhysicalSurface ImpactSurfaceTypes);

	UFUNCTION(BlueprintCallable)
	void AddFakeProjectileData(int32 _NumberOfFakeProjectiles, float ConeHalfAngleInDegrees = 5.f);
	
	//Should we fake projectile data (needed for weapons with multiple pellets like shotguns for now)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedsFakeProjectileDat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfFakeProjectiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APG_WeaponFire> WeaponFireActor;
	UPROPERTY()
	APG_WeaponFire* WeaponFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UNiagaraSystem* NS_ShellEject;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UNiagaraSystem* NS_MuzzleFlash;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UNiagaraSystem* NS_Tracer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shell Eject")
	bool EnableShellEject = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shell Eject")
	UNiagaraSystem* ShellEjectSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shell Eject")
	UStaticMesh* ShellEjectMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Flasht")
	bool EnableMuzzleFlash;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Flash")
	UNiagaraSystem* MuzzleFlashSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracer")
	bool EnableTracer = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracer")
	UNiagaraSystem* TracerSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShellEjectTrigger;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MuzzleFlashTrigger;
};
