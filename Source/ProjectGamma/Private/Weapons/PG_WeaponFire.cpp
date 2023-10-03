// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PG_WeaponFire.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"


// Sets default values
APG_WeaponFire::APG_WeaponFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;
	
}

// Called when the game starts or when spawned
void APG_WeaponFire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APG_WeaponFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APG_WeaponFire::Init(USkeletalMeshComponent* _SkeletalMeshComponent, bool _EnableShellEject,
	UNiagaraSystem* _ShellEjectSystem, bool _EnableMuzzleFlash, UNiagaraSystem* _MuzzleFlashSystem, bool _EnableTracer,
	UNiagaraSystem* _TracerSystem, UStaticMesh* _ShellEjectMesh)
{
	SkeletalMeshComponent =_SkeletalMeshComponent;
	EnableShellEject = _EnableShellEject;
	ShellEjectSystem = _ShellEjectSystem;
	EnableMuzzleFlash = _EnableMuzzleFlash;
	MuzzleFlashSystem = _MuzzleFlashSystem;
	EnableTracer= _EnableTracer;
	TracerSystem = _TracerSystem;
	ShellEjectMesh = _ShellEjectMesh;
}

void APG_WeaponFire::Fire(TArray<FVector>_ImpactPositions, FVector _ImpactNormals, EPhysicalSurface ImpactSurfaceTypes)
{
	if(EnableShellEject)
	{
		
	}
	if(EnableMuzzleFlash)
	{
		
	}
	if(EnableTracer)
	{
		if(!NS_Tracer ||(NS_Tracer && !NS_Tracer->IsActive()))
		{
			FTransform MuzzleTransform = SkeletalMeshComponent->GetSocketTransform("muzzle", ERelativeTransformSpace::RTS_Actor);
			NS_Tracer =  UNiagaraFunctionLibrary::SpawnSystemAttached(TracerSystem, Root, NAME_None, MuzzleTransform.GetLocation(), FRotator(0.f,90.f,0.f), EAttachLocation::Type::KeepRelativeOffset, true,true);
			TracerTrigger = false;
		}
		if(NS_Tracer)
		{
			//Send trigger update to systems
			TracerTrigger = !TracerTrigger;
			NS_Tracer->SetNiagaraVariableBool("User.Trigger", TracerTrigger);
			//send impact position array to niagara system
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NS_Tracer,"User.ImpactPositions", _ImpactPositions);
		}
	}
}

