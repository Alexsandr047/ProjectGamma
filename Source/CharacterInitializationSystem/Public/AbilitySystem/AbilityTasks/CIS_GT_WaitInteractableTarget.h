// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CIS_GT_WaitInteractableTarget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitInteractableTargetDelegate, const FGameplayAbilityTargetDataHandle&, Data);

class AActor;

USTRUCT()
struct FDistanceObjectInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	AActor* Object;

	float Distance;

	FDistanceObjectInfo(): Object(nullptr), Distance(0.f)
	{}
};


/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_GT_WaitInteractableTarget : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	
	//UCIS_GT_WaitInteractableTarget(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintAssignable)
	FWaitInteractableTargetDelegate FoundNewInteractableTarget;

	UPROPERTY(BlueprintAssignable)
	FWaitInteractableTargetDelegate LostInteractableTarget;

	/**
	* Traces a line on a timer looking for InteractableTargets.
	* @param MaxRange How far to trace.
	* @param TimerPeriod Period of trace timer.
	* @param bShowDebug Draws debug lines for traces.
	*/
	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
	static UCIS_GT_WaitInteractableTarget* WaitForInteractableTarget(UGameplayAbility* OwningAbility, FName TaskInstanceName, FCollisionProfileName TraceProfile,
		float TraceRange = 200.0f, float TimerPeriod = 0.1f, bool bShowDebug = true, EDrawDebugTrace::Type SphereDebugTrace = EDrawDebugTrace::None);

	virtual void Activate() override;

protected:
	
	FGameplayAbilityTargetingLocationInfo StartLocation;
	FGameplayAbilityTargetingLocationInfo StartLocation1P;
	FGameplayAbilityTargetingLocationInfo StartLocation3P;

	float TraceRange;
	float TimerPeriod;
	bool bShowDebug;
	EDrawDebugTrace::Type  SphereDebugTrace;

	
	FCollisionProfileName TraceProfile;
	ETraceTypeQuery CollisionTraceTypes;
	FGameplayAbilityTargetDataHandle TargetData;

	FTimerHandle TraceTimerHandle;
	
	UFUNCTION()
	void FindPreferObject();
	
private:

	bool CheckImpactVisible(const AActor* Object, TArray<FDistanceObjectInfo> ActorsList, FVector ViewLocation, FHitResult& PointHit);
	
	void GetImpactObjectBounds(const AActor* Object, FVector& Center, FVector& Extents) const;

	FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const;	
};
