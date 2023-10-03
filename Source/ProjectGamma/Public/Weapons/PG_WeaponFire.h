// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chaos/ChaosEngineInterface.h"
#include "PG_WeaponFire.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
UCLASS()
class PROJECTGAMMA_API APG_WeaponFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_WeaponFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* Root;
	
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	UPROPERTY()
	UNiagaraComponent* NS_ShellEject;
	
	UPROPERTY()
	UNiagaraComponent* NS_MuzzleFlash;
	
	UPROPERTY()
	UNiagaraComponent* NS_Tracer;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TracerTrigger;
	
	UFUNCTION(BlueprintCallable)
	void Init(USkeletalMeshComponent* _SkeletalMeshComponent = nullptr, bool _EnableShellEject = false, UNiagaraSystem*_ShellEjectSystem = nullptr, bool _EnableMuzzleFlash = false, UNiagaraSystem* _MuzzleFlashSystem = nullptr,
		bool _EnableTracer = false, UNiagaraSystem* _TracerSystem = nullptr, UStaticMesh* _ShellEjectMesh = nullptr) ;

	UFUNCTION(BlueprintCallable)
	void Fire(TArray<FVector> _ImpactPositions, FVector _ImpactNormals,EPhysicalSurface _ImpactSurfaceTypes);
};
