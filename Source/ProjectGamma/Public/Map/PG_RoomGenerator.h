// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_RoomActor.h"
#include "GameFramework/Actor.h"
#include "PG_RoomGenerator.generated.h"


class UPG_Base_RoomList;

UCLASS()
class PROJECTGAMMA_API URG_Room : public UObject
{
	GENERATED_BODY()
	
public:

	FVector2D gridPos;
	int32 type;
	bool doorTop;
	bool doorBot;
	bool doorLeft;
	bool doorRight;
	void InitRoom(FVector2D _gridPos, int32 _type);
	bool IsEndRoom() const;
};

UENUM(BlueprintType)
enum class ERoom_Type  : uint8
{
	OneDoor,
	TwoDoorsStraight,
	TwoDoorsAngle,
	ThreeDoors,
	FourDoors
};

USTRUCT(BlueprintType)
struct FRoom_Data
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RoomRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RoomWeight;

	FRoom_Data()
	{
		RoomRef = "";
		RoomWeight = 1.f;
	}

};
USTRUCT(BlueprintType)
struct FRoom_Data_Transform : public FRoom_Data
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;

	FRoom_Data_Transform()
	{
		RoomRef = "";
		RoomWeight = 1.f;
		Location = FVector();
		Rotation = FRotator();
	}
	FRoom_Data_Transform(FRoom_Data _RoomData, FVector _Location, FRotator _Rotation)
	{
		RoomRef = _RoomData.RoomRef;
		RoomWeight = _RoomData.RoomWeight;
		Location = _Location;
		Rotation = _Rotation;
	}
};

UCLASS()
class PROJECTGAMMA_API APG_RoomGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	APG_RoomGenerator();

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_D;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
    TSubclassOf<APG_RoomActor> Room_LD;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
    TSubclassOf<APG_RoomActor> Room_R;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
    TSubclassOf<APG_RoomActor> Room_RD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_RL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
    TSubclassOf<APG_RoomActor> Room_RLD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_RU;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_RUD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_RUL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_RULD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_U;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_UD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_UL;                                      	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<APG_RoomActor> Room_ULD;
	 */
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category ="Rooms|Size")
	FVector2D WorldSize = FVector2D(4.f, 4.f);
	TArray<TArray<URG_Room*>> Rooms;
	TArray<FVector2D> TakenPositions;
	int32 GridSizeX;
	int32 GridSizeY;
	int32 NumberOfRooms = 20;
	
	UPROPERTY(ReplicatedUsing = OnRep_Start)
	int32 RandomSeed;
	UFUNCTION()
	void OnRep_Start();
	
	FRandomStream Stream;
	void MakeRandomSeed();
	void Start();
	void CreateRooms();
	FVector2D NewPosition();
	FVector2D SelectiveNewPosition(); // method differs from the above in the two commented ways
	int32 NumberOfNeighbors(FVector2D CheckingPos, TArray<FVector2D> UsedPositions);
	void MakeSpecialRoom();
	void SetRoomDoors();
	void DrawMap();

	/**
	 * @param TypeRoom if TypeRoom = 0 - Standard  room, 1 - Start room, 2 - Boss Room, 3 - Finish Room, 4 - Secret Room
	 * */
	FRoom_Data_Transform GetRoomActor(int32 TypeRoom, bool Right, bool Up, bool Left, bool Down);
	FRoom_Data MakeRandomRoom(const TArray<FRoom_Data>& Rooms);
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	UPROPERTY(Replicated)
	UPG_Base_RoomList* RoomLostAsset;
	
public:
	
	void Init(UPG_Base_RoomList* RoomList);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
