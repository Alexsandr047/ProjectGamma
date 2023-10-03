// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PG_AttachWeaponToSocketAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_AttachWeaponToSocketAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPG_AttachWeaponToSocketAnimNotify();
	
	UPROPERTY(BlueprintReadWrite ,EditAnywhere)
	FName SocketName;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToSocket(USkeletalMeshComponent* MeshComp);
};

