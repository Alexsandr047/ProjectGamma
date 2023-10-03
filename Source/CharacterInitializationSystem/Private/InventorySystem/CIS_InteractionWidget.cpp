// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/CIS_InteractionWidget.h"
#include "InventorySystem/CIS_Interactable.h"
#include "Kismet/GameplayStatics.h"

UCIS_InteractionWidget::UCIS_InteractionWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Target = nullptr;
}

void UCIS_InteractionWidget::SetTarget_Implementation(UObject* NewTarget)
{
	Target = NewTarget;
	NewObjectReceived();
}



void UCIS_InteractionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	NewObjectReceived();
}



void UCIS_InteractionWidget::GetObjectToSreenPostion(FVector2D& Position) const
{
	if (Target)
	{
		FVector2D ScreenPosition;
		FVector TargetLocation;
		GetObjectLocation(TargetLocation);
		UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), TargetLocation,ScreenPosition, true);
		Position = ScreenPosition;
	}

}

void UCIS_InteractionWidget::GetObjectLocation_Implementation(FVector& Output) const
{
	if (const AActor* Actor = Cast<AActor>(Target))
	{
		Output = Actor->GetActorLocation();
	}
	else if (const UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(Target))
	{
		Output = Comp->GetComponentLocation();
	}	
}
