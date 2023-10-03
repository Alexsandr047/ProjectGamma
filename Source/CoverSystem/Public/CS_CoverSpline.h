// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CS_CoverSpline.generated.h"


class USplineComponent;
class USphereComponent;

USTRUCT(BlueprintType)
struct FSplineCounts : public FTableRowBase {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SplineCounts")
	int SplineCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SplineCounts")
	FName PointName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SplineCounts")
	FVector SpineLocation;
};


UCLASS()
class COVERSYSTEM_API ACS_CoverSpline : public AActor
{
	GENERATED_BODY()
	
public:	

	ACS_CoverSpline();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USplineComponent* CoverSpine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoverSystem|CoverSpline")
	bool LoopingCover = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoverSystem|CoverSpline")
	bool HiddenSphere = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoverSystem|CoverSpline")
	float SphereRadius = 20;
	
protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TArray<FSplineCounts> SplineCounts;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Config")
	TArray<FSplineCounts> GetSplineCounts() const;

	/*Массив, содержащий ссылки на порожденные компоненты.
	Он будет использоваться для удаления старых компонентов в случае, если мы решим
	изменить параметр NumToSpawn*/
	TArray<USphereComponent*> SpArray;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
