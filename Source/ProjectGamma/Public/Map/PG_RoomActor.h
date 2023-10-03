// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "PG_RoomActor.generated.h"

UCLASS()
class PROJECTGAMMA_API APG_RoomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_RoomActor();

	UFUNCTION()
	void SetParam(int32 PosX, int32 PosY, int32 SizeX, int32 SizeY);

	UFUNCTION()
	void SetIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	int32 GetIndex() const { return IndexRoom; }
	
	UPROPERTY()
	int32 IndexRoom = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite/*, Replicated*/)
	UStaticMeshComponent* AttachableMesh;

	
};
