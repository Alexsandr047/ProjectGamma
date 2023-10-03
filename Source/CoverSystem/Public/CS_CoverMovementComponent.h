// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CS_CoverSpline.h"
#include "Components/ActorComponent.h"
#include "CS_CoverMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COVERSYSTEM_API UCS_CoverMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCS_CoverMovementComponent();


	/*Пердвижение в укрытие*/
	void CoverMovement(ACharacter* Character, float Input);


	UFUNCTION(BlueprintCallable, Category = "ProjectGamma|Cover")
	void SetIsInCover(bool bNewIsInCover);

	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ProjectGamma|Cover")
	bool MoveStop = true;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ProjectGamma|Cover")
	bool RightMove = true;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "ProjectGamma|Cover")
	bool bIsInCover = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectGamma|Cover")
	TArray<FSplineCounts> SplineCounts;

	UPROPERTY()
	int CoverPoints;

	UPROPERTY()
	FSplineCounts CurrentPoint;
	UPROPERTY()
	FSplineCounts PreviousPoint;
	UPROPERTY()
	FSplineCounts NextPoint;
	//UPROPERTY(Replicated, BlueprintReadOnly, Category = "ProjectGamma|Cover")
	FRotator CurrentDirectionRotate;
	
public:	

	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
