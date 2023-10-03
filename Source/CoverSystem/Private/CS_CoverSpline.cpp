// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_CoverSpline.h"

#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"

ACS_CoverSpline::ACS_CoverSpline()
{

	PrimaryActorTick.bCanEverTick = false;

	CoverSpine = CreateDefaultSubobject<USplineComponent>(TEXT("Cover Spline"));
	CoverSpine->SetupAttachment(RootComponent);
	
}

void ACS_CoverSpline::OnConstruction(const FTransform& Transform)
{
	//Опустошить массив и удалить все его компоненты
	for (auto It = SpArray.CreateIterator(); It; It++)
	{
		(*It)->DestroyComponent();
	}

	SplineCounts.Empty();

	RegisterAllComponents();

	FName InitialSphereName = FName("MySphereCompName");

	float SplineLength = CoverSpine->GetSplineLength();
	float SphereDiameter = SphereRadius * 2;
	int SplineSections = FMath::Floor(SplineLength / SphereDiameter);

	for (int SplineCount = 0; SplineCount < (SplineSections); SplineCount++)
	{
		FString StrSphere = "MySphereCompName" + FString::FromInt(SplineCount);
		//Преобразование FString в FName
		InitialSphereName = (*StrSphere);
		const FVector Possition = CoverSpine->GetLocationAtDistanceAlongSpline(SphereDiameter * SplineCount, ESplineCoordinateSpace::Local);
		const FVector WorldPossition = CoverSpine->GetLocationAtDistanceAlongSpline(SphereDiameter * SplineCount, ESplineCoordinateSpace::World);
		USphereComponent* NewSphere = NewObject<USphereComponent>(this, InitialSphereName);
		
		//Добавьте ссылку в наш массив
		SpArray.Add(NewSphere);		

		if (NewSphere)
		{
			NewSphere->RegisterComponent();
			NewSphere->SetSphereRadius(SphereRadius);
			NewSphere->SetRelativeLocation(Possition);
			NewSphere->AttachToComponent(CoverSpine, FAttachmentTransformRules::KeepRelativeTransform);
			NewSphere->bHiddenInGame = HiddenSphere;
			NewSphere->SetCollisionProfileName("CoverSlpine");

			FSplineCounts Point;
			Point.SplineCount = SplineCount;
			Point.PointName = InitialSphereName;
			Point.SpineLocation = WorldPossition;
			SplineCounts.Add(Point);
		}
	}
}

TArray<FSplineCounts> ACS_CoverSpline::GetSplineCounts() const
{
	return SplineCounts;
}

void ACS_CoverSpline::BeginPlay()
{
	Super::BeginPlay();
	
}
