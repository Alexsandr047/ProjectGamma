// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PG_ShipBase_MoveCamera.generated.h"

class APG_ShipBase;
/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_ShipBase_MoveCamera : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera|Border")
	float ViewportTopBorder = 0.025;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera|Border")
	float ViewportDownBorder = 0.975;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera|Border")
	float ViewportLeftBorder = 0.025;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera|Border")
	float ViewportRightBorder = 0.975;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera|Lock")
	float ViewportTopLock = 2100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera|Lock")
	float ViewportDownLock = 300;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera|Lock")
	float ViewportLeftLock = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera|Lock")
	float ViewportRightLock = -2000;


	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera")
	float MouseMoveSensitivity = 1.f;
	
	UFUNCTION(BlueprintCallable)
	void Init(APlayerController* Controller);
	
	UPROPERTY()
	APlayerController * PlayerController;

	UPROPERTY()
	APG_ShipBase* ShipBase;

	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	
	void MoveCamera(FVector2f Position);
};
