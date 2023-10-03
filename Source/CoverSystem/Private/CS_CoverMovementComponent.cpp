// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_CoverMovementComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

UCS_CoverMovementComponent::UCS_CoverMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UCS_CoverMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, bIsInCover);
	//DOREPLIFETIME(ThisClass, CurrentDirectionRotate);
}


void UCS_CoverMovementComponent::CoverMovement(ACharacter* Character, float Input)
{
	if(!Character)
		return;
	
	if (Input != 0.f)
	{
		MoveStop = false;
		float Distance = FVector::Distance(Character->GetActorLocation(), CurrentPoint.SpineLocation);
		FVector CoverDirection = UKismetMathLibrary::GetDirectionUnitVector(Character->GetActorLocation(), CurrentPoint.SpineLocation);
		if (Input > 0.f)
		{				
			RightMove = true;
			
			if (Distance > 4.f)
			{				
				Character->AddMovementInput(CoverDirection, 1.f);
				CurrentDirectionRotate = UKismetMathLibrary::FindLookAtRotation(PreviousPoint.SpineLocation, CurrentPoint.SpineLocation);
			}
			else
			{
				if ((CoverPoints-1) != CurrentPoint.SplineCount)
				{
					PreviousPoint = CurrentPoint;
					CurrentPoint = SplineCounts[(CurrentPoint.SplineCount +1)];
					CoverDirection = UKismetMathLibrary::GetDirectionUnitVector(Character->GetActorLocation(), CurrentPoint.SpineLocation);
					Character->AddMovementInput(CoverDirection, 1.f);
					CurrentDirectionRotate = UKismetMathLibrary::FindLookAtRotation(PreviousPoint.SpineLocation, CurrentPoint.SpineLocation);
				}
			}
		}
		else 
		{
			RightMove = false;
			if (Distance > 4.f)
			{
				Character->AddMovementInput(CoverDirection, 1.f);
				CurrentDirectionRotate = UKismetMathLibrary::FindLookAtRotation(CurrentPoint.SpineLocation, NextPoint.SpineLocation);
			}
			else
			{
				if(CurrentPoint.SplineCount != 0)
				{
					NextPoint = CurrentPoint;
					CurrentPoint = SplineCounts[(CurrentPoint.SplineCount - 1)];
					CoverDirection = UKismetMathLibrary::GetDirectionUnitVector(Character->GetActorLocation(), CurrentPoint.SpineLocation);
					Character->AddMovementInput(CoverDirection, 1.f);
					CurrentDirectionRotate = UKismetMathLibrary::FindLookAtRotation(CurrentPoint.SpineLocation, NextPoint.SpineLocation);
				}
			}
		}
	}
	else
	{
		MoveStop = true;
	}
}

void UCS_CoverMovementComponent::SetIsInCover(bool bNewIsInCover)
{
	bIsInCover = bNewIsInCover;
}

void UCS_CoverMovementComponent::BeginPlay()
{
	Super::BeginPlay();	
}


/*
void UCS_CoverMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
*/

