// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PG_ShipBase_MoveCamera.h"
#include "Character/PG_ShipBase.h"

void UPG_ShipBase_MoveCamera::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPG_ShipBase_MoveCamera::Init(APlayerController* Controller)
{

	if(Controller)
		PlayerController = Controller;

	if(PlayerController)
		ShipBase = Cast<APG_ShipBase>(PlayerController->GetPawn());
}

FReply UPG_ShipBase_MoveCamera::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//MoveCamera(FVector2f(InMouseEvent.GetScreenSpacePosition().Y,InMouseEvent.GetScreenSpacePosition().X));
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UPG_ShipBase_MoveCamera::MoveCamera(FVector2f Position)
{

	/*if(!PlayerController)
		PlayerController = Cast<APlayerController>();*/

	if(PlayerController && ShipBase)
	{
		int32 ViewportX = 0;
		int32 ViewportY = 0;
		PlayerController->GetViewportSize(ViewportX, ViewportY);
		
		bool bLockLeft = false;
		bool bLockRight = false;
		bool bLockTop = false;
		bool bLockDown = false;
		FVector ShipCameraLocation = ShipBase->GetActorLocation();
		if(ShipCameraLocation.Y <= ViewportRightLock)
			bLockRight = true;
		if(ShipCameraLocation.Y >= ViewportLeftLock)
			bLockLeft = true;
		if(ShipCameraLocation.Z >= ViewportTopLock)
			bLockTop = true;
		if(ShipCameraLocation.Z <= ViewportDownLock)
			bLockDown = true;
		
		float MoveY = 0.f;
		float MoveZ = 0.f;
		
		if( Position.Y / ViewportY >= ViewportDownBorder && !bLockDown)
			MoveZ = MouseMoveSensitivity * -5;
		if( Position.Y / ViewportY <= ViewportTopBorder && !bLockTop)
			MoveZ =MouseMoveSensitivity * 5;

		if( Position.X / ViewportX >= ViewportRightBorder && !bLockRight)
			MoveY = MouseMoveSensitivity * 5;
		if( Position.X / ViewportX <= ViewportLeftBorder && !bLockLeft)
			MoveY =MouseMoveSensitivity * -5;
		
		ShipBase->AddActorLocalOffset(FVector(0.f, MoveY, MoveZ));
	}
}
