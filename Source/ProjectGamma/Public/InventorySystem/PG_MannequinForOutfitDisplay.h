// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PG_MannequinForOutfitDisplay.generated.h"

UCLASS()
class PROJECTGAMMA_API APG_MannequinForOutfitDisplay : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_MannequinForOutfitDisplay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
