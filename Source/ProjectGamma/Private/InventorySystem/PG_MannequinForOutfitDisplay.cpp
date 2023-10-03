// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/PG_MannequinForOutfitDisplay.h"

// Sets default values
APG_MannequinForOutfitDisplay::APG_MannequinForOutfitDisplay()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Item Skeletal Mesh"));
	SkeletalMeshComponent->bVisibleInReflectionCaptures = true;
	SkeletalMeshComponent->bCastHiddenShadow = true;
	SkeletalMeshComponent->SetTickGroup(TG_PostUpdateWork);
	SkeletalMeshComponent->SetCollisionProfileName("No Collision");
	SkeletalMeshComponent->SetVisibility(true, true);
	
	RootComponent = SkeletalMeshComponent;
}

// Called when the game starts or when spawned
void APG_MannequinForOutfitDisplay::BeginPlay()
{
	Super::BeginPlay();
	
}
