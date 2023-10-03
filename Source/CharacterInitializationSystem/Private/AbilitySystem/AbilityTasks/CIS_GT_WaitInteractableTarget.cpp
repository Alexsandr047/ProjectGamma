// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/CIS_GT_WaitInteractableTarget.h"

#include "Character/CIS_Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine.h"
#include "Components/PrimitiveComponent.h"
#include "Chaos/ChaosPerfTest.h"
#include "InventorySystem/CIS_Interactable.h"
#include "Kismet/KismetSystemLibrary.h"


UCIS_GT_WaitInteractableTarget::UCIS_GT_WaitInteractableTarget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

UCIS_GT_WaitInteractableTarget* UCIS_GT_WaitInteractableTarget::WaitForInteractableTarget(
	UGameplayAbility* OwningAbility, FName TaskInstanceName, FCollisionProfileName TraceProfile, float TraceRange,
	float TimerPeriod, bool bShowDebug, EDrawDebugTrace::Type SphereDebugTrace)
{
	UCIS_GT_WaitInteractableTarget* MyObj = NewAbilityTask<UCIS_GT_WaitInteractableTarget>(OwningAbility, TaskInstanceName);		//Register for task list here, providing a given FName as a key
	MyObj->TraceProfile = TraceProfile;
	MyObj->TraceRange = TraceRange;
	MyObj->TimerPeriod = TimerPeriod;
	MyObj->bShowDebug = bShowDebug;
	MyObj->SphereDebugTrace = SphereDebugTrace;

	ACIS_Character* Hero = Cast<ACIS_Character>(OwningAbility->GetCurrentActorInfo()->AvatarActor);

	/*
	MyObj->StartLocation1P = FGameplayAbilityTargetingLocationInfo();
	MyObj->StartLocation1P.LocationType = EGameplayAbilityTargetingLocationType::SocketTransform;
	MyObj->StartLocation1P.SourceComponent = Hero->GetFirstPersonMesh();
	MyObj->StartLocation1P.SourceSocketName = "WeaponPoint";
	MyObj->StartLocation1P.SourceAbility = OwningAbility;
	*/

	MyObj->StartLocation3P = FGameplayAbilityTargetingLocationInfo();
	MyObj->StartLocation3P.LocationType = EGameplayAbilityTargetingLocationType::SocketTransform;
	MyObj->StartLocation3P.SourceComponent = Hero->GetMesh();
	MyObj->StartLocation3P.SourceSocketName = "Head";
	MyObj->StartLocation3P.SourceAbility = OwningAbility;
	
	return MyObj;
}

void UCIS_GT_WaitInteractableTarget::Activate()
{
	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TraceTimerHandle, this, &UCIS_GT_WaitInteractableTarget::FindPreferObject, TimerPeriod, true);
}

void UCIS_GT_WaitInteractableTarget::FindPreferObject()
{
	bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	AActor* SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	if (!SourceActor)
	{
		// Hero is dead
		//UE_LOG(LogTemp, Error, TEXT("%s %s SourceActor was null"), *FString(__FUNCTION__), *UGSBlueprintFunctionLibrary::GetPlayerEditorWindowRole(GetWorld()));
		return;
	}
	ActorsToIgnore.Add(SourceActor);
	
	// Check player's perspective, could be 1P or 3P
	ACIS_Character* Hero = Cast<ACIS_Character>(SourceActor);
	StartLocation = StartLocation3P;
	/*if (Hero->IsInFirstPersonPerspective())
	{
		StartLocation = StartLocation1P;
	}
	else
	{
		StartLocation = StartLocation3P;
	}*/

	// Calculate TraceEnd
	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	FVector TraceEnd;
	
	//AimWithPlayerController(SourceActor, Params, TraceStart, TraceEnd); //Effective on server and launching client only

	// ------------------------------------------------------

	TArray<FHitResult> HitArray;
	
	const bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceStart, TraceEnd, TraceRange, CollisionTraceTypes, true, ActorsToIgnore,
		SphereDebugTrace,HitArray,true, FLinearColor::Gray ,FLinearColor:: Blue, 2.f );

	if(Hit)
	{
		AActor* Result = nullptr;

		APlayerController* PlayerController = Hero->GetController<APlayerController>();
		if (!PlayerController)
			return;

		FVector ViewLocation;
		FRotator ViewRotation;
		PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
		
		FVector2D ViewportCenter;

		if ( GEngine && GEngine->GameViewport )
		{
			GEngine->GameViewport->GetViewportSize( /*out*/ViewportCenter );
		}
		ViewportCenter = ViewportCenter / 2;
		
		TArray<FDistanceObjectInfo> ActorsList;
		
		const float MaxRadius = FMath::Min(ViewportCenter.X,  ViewportCenter.Y) * 0.9f;

		FHitResult ReturnHitResult;
		
		for(const auto& HitResult : HitArray)
		{
			// Component/Actor must be available to interact
			bool bIsInteractable = HitResult.GetActor()->Implements<UCIS_Interactable>();
			
			if (bIsInteractable && ICIS_Interactable::Execute_IsAvailableForInteraction(HitResult.GetActor(), HitResult.GetComponent()))
			{
				FVector Center;
				FVector Extents;
				GetImpactObjectBounds(HitResult.GetActor(), Center, Extents);
				FVector2D ActorScreenLocation;
				if (UGameplayStatics::ProjectWorldToScreen(PlayerController, Center, ActorScreenLocation))
				{
					const float Distance = FVector2D::Distance(ViewportCenter, ActorScreenLocation);
					if (Distance < MaxRadius/* && Distance < NearestDistance*/)
					{
						ActorsList.AddDefaulted();
						ActorsList.Last().Object = HitResult.GetActor();
						ActorsList.Last().Distance = Distance;
					}
				}

				ActorsList.Sort([](const FDistanceObjectInfo& InfoA, const FDistanceObjectInfo& InfoB)
				{
					return InfoA.Distance < InfoB.Distance;
				});

				for (const FDistanceObjectInfo& Info : ActorsList)
				{
					if (CheckImpactVisible(Info.Object, ActorsList, ViewLocation,ReturnHitResult))
					{
						Result = Info.Object;
						break;
					}
				}
			}
		}
		
		if (!ReturnHitResult.bBlockingHit)
		{
			// No valid, available Interactable Actor

			ReturnHitResult.Location = TraceEnd;
			if (TargetData.Num() > 0 && TargetData.Get(0)->GetHitResult()->GetActor())
			{
				// Previous trace had a valid Interactable Actor, now we don't have one
				// Broadcast last valid target
				LostInteractableTarget.Broadcast(TargetData);
			}

			TargetData = MakeTargetData(ReturnHitResult);
		}
		else
		{
			// Valid, available Interactable Actor

			bool bBroadcastNewTarget = true;

			if (TargetData.Num() > 0)
			{
				const AActor* OldTarget = TargetData.Get(0)->GetHitResult()->GetActor();

				if (OldTarget == ReturnHitResult.GetActor())
				{
					// Old target is the same as the new target, don't broadcast the target
					bBroadcastNewTarget = false;
				}
				else if (OldTarget)
				{
					// Old target exists and is different from the new target
					// Broadcast last valid target
					LostInteractableTarget.Broadcast(TargetData);
				}
			}

			if (bBroadcastNewTarget)
			{
				// Broadcast new valid target
				TargetData = MakeTargetData(ReturnHitResult);
				FoundNewInteractableTarget.Broadcast(TargetData);
			}
		}
	}
}

	
bool UCIS_GT_WaitInteractableTarget::CheckImpactVisible(const AActor* Object, TArray<FDistanceObjectInfo> ActorsList, FVector ViewLocation, FHitResult& PointHit)
{
		
	FVector ObjectBoxCenter;
	FVector ObjectBoxExtent;
	GetImpactObjectBounds(Object, ObjectBoxCenter, ObjectBoxExtent);

	const FVector HalfSize = ObjectBoxExtent / 2.f;
	TArray<FVector> PointsList({
			ObjectBoxCenter + HalfSize * FVector::UpVector,
			ObjectBoxCenter + HalfSize * FVector::LeftVector,
			ObjectBoxCenter + HalfSize * FVector::RightVector,
			ObjectBoxCenter + HalfSize * FVector::ForwardVector,
			ObjectBoxCenter + HalfSize * FVector::BackwardVector,
			ObjectBoxCenter + HalfSize * FVector::DownVector
	});

	TArray<AActor*> OverlappingActors;
	OverlappingActors.Empty();

	for(const auto& Actor: ActorsList)
	{
		if(Actor.Object && Actor.Object != Object)
		{
			OverlappingActors.Add(Actor.Object);
		}
	}
	
	for (const FVector& Point : PointsList)
	{
		FHitResult PointHitResult;
		UKismetSystemLibrary::LineTraceSingle(this, Point, ViewLocation,
				UEngineTypes::ConvertToTraceType(ECC_Visibility),
				false, OverlappingActors, SphereDebugTrace, PointHitResult, true,FLinearColor::Gray ,FLinearColor:: Blue, 2.f );

		if (PointHitResult.bBlockingHit)
		{
			PointHit = PointHitResult;
			return true;
		}
	}

	return false;
}

void UCIS_GT_WaitInteractableTarget::GetImpactObjectBounds(const AActor* Object, FVector& Center,
                                                           FVector& Extents) const
{
	if (Object->IsA(AActor::StaticClass()))
	{
		const AActor* Actor = Cast<AActor>(Object);
		Actor->GetActorBounds(true, Center, Extents, true);
	}
	else if (Object->IsA(UPrimitiveComponent::StaticClass()))
	{
		const UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Object);
		PrimitiveComponent->Bounds.GetBox().GetCenterAndExtents(Center, Extents);
	}
}

FGameplayAbilityTargetDataHandle UCIS_GT_WaitInteractableTarget::MakeTargetData(const FHitResult& HitResult) const
{
	/** Note: This will be cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
	return StartLocation.MakeTargetDataHandleFromHitResult(Ability, HitResult);
}




	
