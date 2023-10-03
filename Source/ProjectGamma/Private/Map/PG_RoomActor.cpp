// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/PG_RoomActor.h"

// Sets default values
APG_RoomActor::APG_RoomActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	AttachableMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Clip TP Mesh"));
	RootComponent = AttachableMesh;

	
}

// Called when the game starts or when spawned
void APG_RoomActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APG_RoomActor::SetParam(int32 PosX, int32 PosY, int32 SizeX, int32 SizeY)
{
	AttachableMesh->SetRelativeLocation(FVector(PosX * 100, PosY* 100, 0.f));
	AttachableMesh->SetRelativeScale3D(FVector(SizeX,SizeY, 1.f));
	
}

void APG_RoomActor::SetIndex(int32 Index)
{
	IndexRoom = Index;
}

