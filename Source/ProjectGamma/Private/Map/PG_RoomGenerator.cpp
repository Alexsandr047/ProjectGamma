// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/PG_RoomGenerator.h"

#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Map/PG_Base_RoomList.h"
#include "Net/UnrealNetwork.h"


void URG_Room::InitRoom(FVector2D _gridPos, int32 _type)
{
	gridPos = _gridPos;
	type = _type;
}

bool URG_Room::IsEndRoom() const
{
	int32 Bools = 0;
	if(doorTop)
		++Bools;
	if(doorBot)
		++Bools;
	if(doorLeft)
		++Bools;
	if(doorRight)
		++Bools;

	if(Bools == 1)
		return true;

	return false;
}

// Sets default values
APG_RoomGenerator::APG_RoomGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RandomSeed = 0;
	
	bReplicates = true;
}
void APG_RoomGenerator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, RandomSeed);
	DOREPLIFETIME(ThisClass,RoomLostAsset);
}

void APG_RoomGenerator::OnRep_Start()
{
	Start();
}

void APG_RoomGenerator::MakeRandomSeed()
{
	if (HasAuthority())
	{
		RandomSeed = UKismetMathLibrary::RandomIntegerInRange(0, 1000);
	}
}

void APG_RoomGenerator::Start()
{
	Stream.Initialize(RandomSeed);
	
	if (NumberOfRooms >= (WorldSize.X * 2) * (WorldSize.Y * 2))
	{ // make sure we dont try to make more rooms than can fit in our grid
		NumberOfRooms = FMath::RoundToInt((WorldSize.X * 2) * (WorldSize.Y * 2));
	}
	GridSizeX = FMath::RoundToInt(WorldSize.X); //note: these are half-extents
	GridSizeY = FMath::RoundToInt(WorldSize.Y);
	CreateRooms(); //lays out the actual map
	SetRoomDoors(); //assigns the doors where rooms would connect
	MakeSpecialRoom();
	DrawMap(); //instantiates objects to make up a map
}

void APG_RoomGenerator::CreateRooms()
{
	Rooms.Empty();
	Rooms.AddDefaulted(GridSizeX * 2);
	for(auto& RoomsX : Rooms)
	{
		RoomsX.AddDefaulted(GridSizeY * 2);
	}
	//Rooms[GridSizeX * 2][GridSizeY * 2];
	URG_Room* StartRoom = NewObject<URG_Room>(URG_Room::StaticClass());
	StartRoom->InitRoom(FVector2D(0, 0), 1);
	Rooms[GridSizeX][GridSizeY] = StartRoom;
	TakenPositions.Add(FVector2D(0, 0));
	FVector2D CheckPos = FVector2D(0, 0);
	//magic numbers
	float RandomCompare = 0.2f, RandomCompareStart = 0.2f, RandomCompareEnd = 0.01f;
	//add rooms
	for (int i =0; i < NumberOfRooms -1; i++)
	{
		float randomPerc =  i / (NumberOfRooms - 1);
		RandomCompare = FMath::Lerp(RandomCompareStart, RandomCompareEnd, randomPerc);
		//grab new position
		CheckPos = NewPosition();
		//test new position
		if (NumberOfNeighbors(CheckPos, TakenPositions) > 1 && UKismetMathLibrary::RandomFloatInRangeFromStream(Stream, 0.f , 1.f) > RandomCompare)
		{
			int32 Iterations = 0;
			do
			{
				CheckPos = SelectiveNewPosition();
				Iterations++;
			}
			while(NumberOfNeighbors(CheckPos, TakenPositions) > 1 && Iterations < 100);
			if (Iterations >= 50)
				UE_LOG(LogTemp, Error, TEXT("ARG_RoomGenerator: could not create with fewer neighbors than :  NumberOfNeighbors (CheckPos: %s, TakenPositions %s)"), *FString::FromInt(NumberOfNeighbors(CheckPos, TakenPositions)));
		}
		//finalize position
		URG_Room* NewRoom = NewObject<URG_Room>(URG_Room::StaticClass());
		NewRoom->InitRoom(CheckPos, 0);
		Rooms[CheckPos.X + GridSizeX][CheckPos.Y + GridSizeY] = NewRoom;
		TakenPositions.Add(CheckPos);
	}	
}

FVector2D APG_RoomGenerator::NewPosition()
{
	int32 x = 0, y = 0;
	FVector2D CheckingPos = FVector2D(0, 0);
	do{
		int32 index = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,0, TakenPositions.Num() - 1); // pick a random room
		x = TakenPositions[index].X;//capture its x, y position
		y = TakenPositions[index].Y;
		bool UpDown = (UKismetMathLibrary::RandomFloatInRangeFromStream(Stream,0.f , 1.f) < 0.5f);//randomly pick wether to look on hor or vert axis
		bool Positive = (UKismetMathLibrary::RandomFloatInRangeFromStream(Stream,0.f , 1.f) < 0.5f);//pick whether to be positive or negative on that axis
		if (UpDown)
		{ //find the position bnased on the above bools
			if (Positive)
				y -= 1;
			else
				y += 1;
		}
		else
		{
			if (Positive)
				x += 1;
			else
				x -= 1;
		}
		CheckingPos = FVector2D(x, y);
	}
	while (TakenPositions.Contains(CheckingPos) || x >= GridSizeX || x < -GridSizeX || y >= GridSizeY || y < -GridSizeY); //make sure the position is valid
	return CheckingPos;
}
FVector2D APG_RoomGenerator::SelectiveNewPosition()
{
	int32 index = 0, inc = 0;
	int32 x =0, y =0;
	FVector2D CheckingPos = FVector2D(0, 0);
	do
	{
		inc = 0;
		do
		{ 
			//instead of getting a room to find an adject empty space, we start with one that only 
			//as one neighbor. This will make it more likely that it returns a room that branches out
			index = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,0, TakenPositions.Num() - 1);
			inc ++;
		}
		while (NumberOfNeighbors(TakenPositions[index], TakenPositions) > 1 && inc < 100);
		x = TakenPositions[index].X;
		y = TakenPositions[index].Y;
		bool UpDown = (UKismetMathLibrary::RandomFloatInRangeFromStream(Stream,0.f , 1.f) < 0.5f);
		bool Positive = (UKismetMathLibrary::RandomFloatInRangeFromStream(Stream,0.f , 1.f) < 0.5f);
		if (UpDown)
		{
			if (Positive)
				y -= 1;
			else
				y += 1;
		}
		else
		{
			if (Positive)
				x += 1;
			else
				x -= 1;
		}
		CheckingPos = FVector2D(x, y);
	}while (TakenPositions.Contains(CheckingPos) || x >= GridSizeX || x < -GridSizeX || y >= GridSizeY || y < -GridSizeY);
	if (inc >= 100) // break loop if it takes too long: this loop isnt garuanteed to find solution, which is fine for this
		UE_LOG(LogTemp, Error, TEXT("ARG_RoomGenerator: could not find position with only one neighbor"));
	
	return CheckingPos;
}
int32 APG_RoomGenerator::NumberOfNeighbors(FVector2D CheckingPos, TArray<FVector2D> UsedPositions)
{
	int32 ret = 0; // start at zero, add 1 for each side there is already a room
	
	if (UsedPositions.Contains(CheckingPos + FVector2D(1, 0))) //using Vector.[direction] as short hands, for simplicity
		ret++;
	
	if (UsedPositions.Contains(CheckingPos + FVector2D(-1, 0)))
		ret++;
	
	if (UsedPositions.Contains(CheckingPos + FVector2D(0, 1)))
		ret++;
	
	if (UsedPositions.Contains(CheckingPos + FVector2D(0, -1)))
		ret++;
	
	return ret;
}

void APG_RoomGenerator::MakeSpecialRoom()
{
	TArray<URG_Room*> EndsRooms;
	EndsRooms.Empty();
	for(auto& RoomX : Rooms)
	{
		for(auto& Room :RoomX)
		{
			if(Room && Room->IsEndRoom())
			{
				EndsRooms.Add(Room);
			}
		}
	}
	float MaxDistance = 0;
	URG_Room* BossRoom = nullptr;
	for(auto& Room : EndsRooms)
	{
		float X = Room->gridPos.X;
		float Y = Room->gridPos.Y;
		float diagonal = UKismetMathLibrary::Sqrt(X*X + Y*Y);
		if(diagonal > MaxDistance)
		{
			MaxDistance = diagonal;
			BossRoom = Room;
		}
	}
	BossRoom->type = 2;
	for(auto& Room : EndsRooms)
	{
		if(Room->type == 0)
		{
			Room->type = 3;
			break;;
		}
	}
	for(auto& Room : EndsRooms)
	{
		if(Room->type == 0)
		{
			Room->type = 4;
			break;
		}
	}
}

void APG_RoomGenerator::SetRoomDoors()
{
	for (int32 x = 0; x < GridSizeX * 2; x++)
	{
		for (int32 y = 0; y < GridSizeY * 2; y++)
		{
			if (Rooms[x][y] == nullptr)
				continue;
			
			if (y - 1 < 0) //check above
				Rooms[x][y]->doorTop = false;
			else
				Rooms[x][y]->doorTop = Rooms[x][y-1] != nullptr;
			
			if (y + 1 >= GridSizeY * 2) //check bellow
				Rooms[x][y]->doorBot = false;
			else
				Rooms[x][y]->doorBot = Rooms[x][y+1] != nullptr;
			
			if (x - 1 < 0) //check left
				Rooms[x][y]->doorLeft = false;
			else
				Rooms[x][y]->doorLeft = Rooms[x-1][y] != nullptr;
			
			if (x + 1 >= GridSizeX * 2) //check right
				Rooms[x][y]->doorRight = false;
			else
				Rooms[x][y]->doorRight = Rooms[x+1][y] != nullptr;
		}
	}
}

void APG_RoomGenerator::DrawMap()
{
	for(auto& RoomX : Rooms)
	{
		for(auto& Room :RoomX)
		{
			if(Room)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				{
					
					auto RoomData = GetRoomActor(Room->type, Room->doorRight,Room->doorTop, Room->doorLeft, Room->doorBot);
					FString RoomName = "X_" + FString::FromInt(Room->gridPos.X) + "_Y_" + FString::FromInt(Room->gridPos.Y);
					bool Succes;
					ULevelStreamingDynamic::LoadLevelInstance(this, RoomData.RoomRef, FVector(RoomLostAsset->RoomsSize * Room->gridPos.X + RoomData.Location.X, RoomLostAsset->RoomsSize * Room->gridPos.Y + RoomData.Location.Y, 0.f), RoomData.Rotation,Succes,RoomName);
					/*APG_RoomActor* RA = GetWorld()->SpawnActor<APG_RoomActor>(GetRoomActor(Room->doorRight,Room->doorTop, Room->doorLeft, Room->doorBot));
					if(RA)
					{
						RA->SetParam(Room->gridPos.X,Room->gridPos.Y,1, 1);
						int32 index = Room->type;
						RA->IndexRoom = index;
					}*/
				}
			}
		}
	}
}

FRoom_Data_Transform APG_RoomGenerator::GetRoomActor(int32 TypeRoom,bool Right, bool Up, bool Left, bool Down)
{
	auto RotateRoom = [this](int32 TypeRotate, FRoom_Data RoomData)
	{
		FRotator Rotation  = FRotator(0.f, 0.f, 0.f);
		FVector Location = FVector(0.f, 0.f, 0.f);
		float RoomsSize = RoomLostAsset->RoomsSize;
		if(TypeRotate == 1)
		{
			Rotation =  FRotator(0.f,90.f,0.f);
			Location = FVector(RoomsSize, 0.f, 0.f);
			return FRoom_Data_Transform(RoomData,Location,Rotation);
		}
		if(TypeRotate == 2)
		{
			Rotation =  FRotator(0.f,180.f,0.f);
			Location = FVector(RoomsSize, RoomsSize, 0.f);
			return FRoom_Data_Transform(RoomData,Location,Rotation);
		}
		if(TypeRotate == 3)
		{
			Rotation =  FRotator(0.f,270.f,0.f);
			Location = FVector(0.f, RoomsSize, 0.f);
			return FRoom_Data_Transform(RoomData,Location,Rotation);
		}
		return FRoom_Data_Transform(RoomData,Location,Rotation);
	};
	if (Up)
	{
		if (Down)
		{
			if (Right)
			{
				if (Left)
				{
					auto Room = MakeRandomRoom(RoomLostAsset->GetFourDoorsRooms(TypeRoom));
					return RotateRoom(UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,0, 3),Room);
					//return Room_RULD;
				}					
				else
				{
					auto Room = MakeRandomRoom(RoomLostAsset->GetThreeDoorsRooms(TypeRoom));
					return RotateRoom(2,Room);
					//return Room_RUD;
				}
			}
			else
			{
				if (Left)
				{
					auto Room = MakeRandomRoom(RoomLostAsset->GetThreeDoorsRooms(TypeRoom));
					return RotateRoom(0,Room);
					//return Room_ULD;
				}
				else
				{
					auto Room = MakeRandomRoom(RoomLostAsset->GetTwoDoorsStraightRooms(TypeRoom));
					if(UKismetMathLibrary::RandomBoolFromStream(Stream))
						return RotateRoom(1,Room);
					return RotateRoom(3,Room);
					//return  Room_UD;
				}
			}
		}
		else
		{
			if (Right)
			{
				if (Left)
				{
					auto Room = MakeRandomRoom(RoomLostAsset->GetThreeDoorsRooms(TypeRoom));
					return RotateRoom(1,Room);
					//return Room_RUL;
				}
				else
				{
					auto Room = MakeRandomRoom(RoomLostAsset->GetTwoDoorsAngleRooms(TypeRoom));
					return RotateRoom(1,Room);
					//return Room_RU;
				}
			}
			else
			{
				if (Left)
				{
					auto Room = MakeRandomRoom(RoomLostAsset->GetTwoDoorsAngleRooms(TypeRoom));
					return RotateRoom(0,Room);
					//return  Room_UL;
				}
				else
				{
					auto Room = MakeRandomRoom(RoomLostAsset->GetOneDoorRooms(TypeRoom));
					return RotateRoom(1,Room);
					//return Room_U;
				}
			}
		}
	}
	if (Down)
	{
		if (Right)
		{
			if(Left)
			{
				auto Room = MakeRandomRoom(RoomLostAsset->GetThreeDoorsRooms(TypeRoom));
				return RotateRoom(3,Room);
				//return Room_RLD;
			}
			else
			{
				auto Room = MakeRandomRoom(RoomLostAsset->GetTwoDoorsAngleRooms(TypeRoom));
				return RotateRoom(2,Room);
				//return Room_RD;
			}
		}
		else
		{
			if (Left)
			{
				auto Room = MakeRandomRoom(RoomLostAsset->GetTwoDoorsAngleRooms(TypeRoom));
				return RotateRoom(3,Room);
				//return Room_LD;
			}
			else
			{
				auto Room = MakeRandomRoom(RoomLostAsset->GetOneDoorRooms(TypeRoom));
				return RotateRoom(3,Room);
				//return Room_D;
			}
		}
	}
	if (Right)
	{
		if (Left)
		{
			auto Room = MakeRandomRoom(RoomLostAsset->GetTwoDoorsStraightRooms(TypeRoom));
			if(UKismetMathLibrary::RandomBoolFromStream(Stream))
				return RotateRoom(0,Room);
			return RotateRoom(2,Room);
			//return Room_RL;
		}
		else
		{
			auto Room = MakeRandomRoom(RoomLostAsset->GetOneDoorRooms(TypeRoom));
			return RotateRoom(2,Room);
			//return  Room_R;
		}
	}
	else
	{
		auto Room = MakeRandomRoom(RoomLostAsset->GetOneDoorRooms(TypeRoom));
		return RotateRoom(0,Room);
		//return Room_L;
	}
}

FRoom_Data APG_RoomGenerator::MakeRandomRoom(const TArray<FRoom_Data>& _Rooms)
{
	int32 RandIndex = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,0, _Rooms.Num()-1);
	if(UKismetMathLibrary::RandomFloatFromStream(Stream) <= _Rooms[RandIndex].RoomWeight)
		return _Rooms[RandIndex];

	return MakeRandomRoom(_Rooms);
}


// Called when the game starts or when spawned
void APG_RoomGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void APG_RoomGenerator::Init(UPG_Base_RoomList* RoomList)
{
	RoomLostAsset = RoomList;
	if(!RoomLostAsset)
		return;
	
	MakeRandomSeed();
	Start();
}

// Called every frame
void APG_RoomGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

