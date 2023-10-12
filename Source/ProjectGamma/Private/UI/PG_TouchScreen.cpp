// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_TouchScreen.h"

#include "Kismet/KismetMathLibrary.h"


void UPG_TouchScreen::NativeConstruct()
{
	Super::NativeConstruct();

	/*if ( GEngine && GEngine->GameViewport )
	{
		GEngine->GameViewport->GetViewportSize( /*out#1#ViewportSize );
	}*/
}

FReply UPG_TouchScreen::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bStartMove = true;
	//LastPosition = InMouseEvent.GetScreenSpacePosition();
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


FReply UPG_TouchScreen::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bStartMove = false;
	TouchMove(FVector2D(0.f, 0.f), false);
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UPG_TouchScreen::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(bStartMove)
	{
		/*FVector Alpha =  FMath::VInterpTo(FVector(StartPosition.X,StartPosition.Y, 0.f), FVector(Position.X, Position.Y, 0.f), DeltaTick,0.1);*/
		FVector2D Position;
		//Position = InMouseEvent.GetScreenSpacePosition();

		FVector2d TouchMoveValue;	

		TouchMoveValue = Position - LastPosition;
		LastPosition = Position;

		float XAbs = UKismetMathLibrary::Abs(TouchMoveValue.X);
		float YAbs = UKismetMathLibrary::Abs(TouchMoveValue.Y);
		if(XAbs > 1 || XAbs < -1)
			TouchMoveValue.X = (TouchMoveValue.X / XAbs);
		
		if(YAbs > 1 || YAbs < -1)
			TouchMoveValue.Y = (TouchMoveValue.Y / YAbs);

		
			TouchMove(TouchMoveValue, true);
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UPG_TouchScreen::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UPG_TouchScreen::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bStartMove = false;
	TouchMove(FVector2D(0.f, 0.f), false);
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UPG_TouchScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
