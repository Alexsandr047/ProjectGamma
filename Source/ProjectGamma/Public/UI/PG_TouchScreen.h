// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PG_TouchScreen.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_TouchScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent ) override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	/*UPROPERTY()
	FVector2D ViewportSize;*/
	
	UPROPERTY()
	bool bStartMove = false;
	UPROPERTY()
	FVector2D LastPosition;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TouchMove(const FVector2D PositionTouch, bool bMoved); 
};
