// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PG_CoreTypes.h"
#include "GameFramework/Actor.h"
#include "PG_Hub_HangarRoom.generated.h"

UCLASS()
class PROJECTGAMMA_API APG_Hub_HangarRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_Hub_HangarRoom();

	/*UFUNCTION(BlueprintCallable)
	TArray<FSquad> GetSquadsData() const { return Squads; }

	UFUNCTION(BlueprintCallable)
	bool CreateSquad(FSquad Squad);

	UFUNCTION(BlueprintCallable)
	bool DeleteSquad(const FString& SquadId);

	UFUNCTION(BlueprintCallable)
	void ChangeSquad(const FString& SquadId, FSquad Squad);*/
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FSquad> Squads;
};
