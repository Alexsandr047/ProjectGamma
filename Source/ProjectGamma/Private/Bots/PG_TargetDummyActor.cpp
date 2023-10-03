// Fill out your copyright notice in the Description page of Project Settings.


#include "Bots/PG_TargetDummyActor.h"

#include "AbilitySystem/AttributeSet/CIS_HealthSet.h"
#include "Bots/PG_TargetDummyHealthSet.h"

APG_TargetDummyActor::APG_TargetDummyActor(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UPG_TargetDummyHealthSet>(TEXT("Health Set")))
{
	MeshComponent = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("Mesh Component"));
	RootComponent = MeshComponent;
}

void APG_TargetDummyActor::InitializeAbilitySystem()
{
	Super::InitializeAbilitySystem();

	// Initialize default values for Health Set

	// This will fail to work if you try to do it before Super::InitializeAbilitySystem()
	// Can't move this to the constructor for that reason.

	HealthSet->InitMaxHealth(100.f);
	HealthSet->InitHealth(1.f);
}

void APG_TargetDummyActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay with Health = %.1f/%.1f"), HealthSet->GetHealth(), HealthSet->GetMaxHealth());
}
