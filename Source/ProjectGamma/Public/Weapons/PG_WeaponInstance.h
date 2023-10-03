// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Equipment/CIS_EquipmentInstance.h"
#include "PG_WeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_WeaponInstance : public UCIS_EquipmentInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite ,EditAnywhere)
	TSubclassOf<UAnimInstance> UnarmedPose;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UAnimInstance> WeaponPose;

	UFUNCTION(BlueprintCallable)
	virtual void OnEquipped() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnUnequipped() override;

	UFUNCTION(BlueprintCallable)
	void SetIsCurrentWeapon(bool IsCurrentWeapon);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsCurrentWeapon() const { return bIsCurrentWeapon;}

	UFUNCTION(BlueprintCallable)
	void UpdateFiringTime();

private:
	UPROPERTY(Replicated)
	bool bIsCurrentWeapon;
	
	UPROPERTY(ReplicatedUsing=OnRep_UseWeaponPose)
	bool bWeaponPose;
	
	UFUNCTION()
	void OnRep_UseWeaponPose();

	UPROPERTY(ReplicatedUsing=OnRep_UseUnarmedPose)
	bool bUnarmedPose;
	
	UFUNCTION()
	void OnRep_UseUnarmedPose();
	
	double TimeLastFired = 0.0;

};
