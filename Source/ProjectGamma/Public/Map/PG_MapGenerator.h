// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_RoomActor.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "PG_MapGenerator.generated.h"


class APG_MapGenerator;
UCLASS()
class PROJECTGAMMA_API UPG_MapLeaf : public UObject
{
	GENERATED_BODY()

public:
	
	const int32 Min_Leaf_Size = 6;
	// the position and size of this Leaf
	UPROPERTY()
	int32 Y;
	UPROPERTY()
	int32 X;
	UPROPERTY()
	int32 Width;
	UPROPERTY()
	int32 Height;
	UPROPERTY()
	UPG_MapLeaf* LeftChild; // the Leaf's left child Leaf
	UPROPERTY()
	UPG_MapLeaf* RightChild; // the Leaf's right child Leaf

	UFUNCTION()
	void InitLeafParam(int32 _X, int32 _Y, int32 _Width, int32 _Height);
	bool SplitLeaf();
	
	UPROPERTY()
	AActor* Owner;
	void CreateRooms(APG_MapGenerator* MapGenerator, TSubclassOf<APG_RoomActor> RoomActor);
	//UPROPERTY()
	//public var room:Rectangle; // the room that is inside this Leaf
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomCreate, APG_RoomActor*, Room);

UCLASS()
class PROJECTGAMMA_API APG_MapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_MapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	
	UPROPERTY()
	TArray<UPG_MapLeaf*> halls; // hallways to connect this Leaf to other Leafs

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APG_RoomActor> RoomActor;

	void GenerateHalls();

	void CorridorsMapCorridors(FVector RoomA, FVector RoomB);
	TArray<FVector>CorridorMakeY(FVector From, FVector To);
	TArray<FVector>CorridorMakeX(FVector From, FVector To);
	
	UPROPERTY(BlueprintAssignable)
	FOnRoomCreate RoomCreate;

	UFUNCTION()
	void AddRoom(APG_RoomActor* Room);
	UFUNCTION()
	void SetRoomsParam();
private:
	const int32 MAX_LEAF_SIZE = 20;

	UPROPERTY()
	TArray<APG_RoomActor*> CreatedRooms;
	
};

