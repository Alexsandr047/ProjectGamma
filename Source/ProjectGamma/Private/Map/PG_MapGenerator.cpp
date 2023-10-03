// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/PG_MapGenerator.h"

#include "Kismet/KismetMathLibrary.h"


void UPG_MapLeaf::InitLeafParam(int32 _X, int32 _Y, int32 _Width, int32 _Height)
{
	Y = _Y;
	X = _X;
	Width = _Width;
	Height = _Height;
}

bool UPG_MapLeaf::SplitLeaf()
{
	// begin splitting the leaf into two children
	if (LeftChild  || RightChild)
		return false; // we're already split! Abort!
	// determine direction of split
	// if the width is >25% larger than height, we split vertically
	// if the height is >25% larger than the width, we split horizontally
	// otherwise we split randomly
	float WidthFloat = Width;
	float HeightFloat = Height;
	bool SplitH = FMath::RandBool();
	if (Width > Height && (WidthFloat / HeightFloat) >= 1.25f)
		SplitH = false;
	else if (Height > Width && HeightFloat / WidthFloat >= 1.25f)
		SplitH = true;
	int32 Max = (SplitH ? Height : Width) - Min_Leaf_Size; // determine the maximum height or width
	if (Max <= Min_Leaf_Size)
		return false; // the area is too small to split any more...
	int32 Split = FMath::RandRange(Min_Leaf_Size, Max); // determine where we're going to split
	// create our left and right children based on the direction of the split
	if (SplitH)
	{
		LeftChild = NewObject<UPG_MapLeaf>();
		LeftChild->Owner = Owner;
		LeftChild->InitLeafParam(X, Y, Width, Split);
		RightChild= NewObject<UPG_MapLeaf>();
		RightChild->Owner = Owner;
		RightChild ->InitLeafParam(X, Y + Split, Width, Height - Split);
	}
	else
	{
		LeftChild = NewObject<UPG_MapLeaf>();
		LeftChild->Owner = Owner;
		LeftChild->InitLeafParam(X, Y, Split, Height);
		RightChild= NewObject<UPG_MapLeaf>();
		RightChild->Owner = Owner;
		RightChild->InitLeafParam(X + Split, Y, Width - Split, Height);
	}
	
	return true;
}

void UPG_MapLeaf::CreateRooms(APG_MapGenerator* MapGenerator, TSubclassOf<APG_RoomActor> RoomActor)
{
	// this function generates all the rooms and hallways for this Leaf and all of its children.
	if (LeftChild || RightChild)
	{
		// this leaf has been split, so go into the children leafs
		if (LeftChild)
		{
			LeftChild->CreateRooms(MapGenerator, RoomActor);
		}
		if (RightChild)
		{
			RightChild->CreateRooms(MapGenerator, RoomActor);
		}
	}
	else
	{
		// this Leaf is the ready to make a room
		FVector2D RoomSize;
		FVector2D RoomPos;
		// the room can be between 3 x 3 tiles to the size of the leaf - 2.
		RoomSize = FVector2d(FMath::RandRange(3, Width-2), FMath::RandRange(3, Height - 2));
		// place the room within the Leaf, but don't put it right 
		// against the side of the Leaf (that would merge rooms together)
		int32 MaxRoomPosX = Width - RoomSize.X - 1;
		int32 MaxRoomPosY = Height - RoomSize.Y - 1;
		RoomPos = FVector2d(FMath::RandRange(1, MaxRoomPosX), FMath::RandRange(1, MaxRoomPosY));
		if(RoomActor)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			if(Owner->GetWorld())
			{
				APG_RoomActor* RA = Owner->GetWorld()->SpawnActorDeferred<APG_RoomActor>(RoomActor, FTransform::Identity, Owner);
				if(RA)
				{
					RA->SetParam(X + RoomPos.X,Y + RoomPos.Y,RoomSize.X, RoomSize.Y);
					MapGenerator->RoomCreate.Broadcast(RA);
				}
			}
		}
		
		
		//room = new Rectangle(x + roomPos.x, y + roomPos.y, roomSize.x, roomSize.y);
	}
}

// Sets default values
APG_MapGenerator::APG_MapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APG_MapGenerator::BeginPlay()
{
	Super::BeginPlay();

	CreatedRooms.Empty();
	RoomCreate.AddDynamic(this, &ThisClass::AddRoom);
	GenerateHalls();
	SetRoomsParam();
}
void APG_MapGenerator::AddRoom(APG_RoomActor* Room)
{
	CreatedRooms.Add(Room);
}

void APG_MapGenerator::SetRoomsParam()
{
	for (int32 i = 0; i < CreatedRooms.Num(); ++ i)
	{
		CreatedRooms[i]->SetIndex(i);
	}
}
void APG_MapGenerator::GenerateHalls()
{
	TArray<UPG_MapLeaf*> Leafs;
	UPG_MapLeaf* Root = NewObject<UPG_MapLeaf>(this);
	Root->Owner = this;
	Root->InitLeafParam(0,0,40,40);
	Leafs.Add(Root);
	bool DidSplit = true;

	while(DidSplit)
	{
		DidSplit = false;

		if(Leafs.Num() > 0)
		{
			TArray<UPG_MapLeaf*> LeftsArray;
			LeftsArray.Empty();
			for(auto& Leaf : Leafs)
			{
				if(Leaf)
				{
					if (!Leaf->LeftChild  &&! Leaf->RightChild) // if this Leaf is not already split...
					{
						// if this Leaf is too big, or 75% chance...
						if (Leaf->Width > MAX_LEAF_SIZE || Leaf->Height > MAX_LEAF_SIZE || FMath::RandRange(1,100) > 25)
						{
							if (Leaf->SplitLeaf()) // split the Leaf!
							{
								LeftsArray.Add(Leaf->LeftChild);
								LeftsArray.Add(Leaf->RightChild);
								DidSplit = true;
								break;
							}
						}
					}
				}
			}
			for(auto& Leaf : LeftsArray)
			{
				Leafs.Add(Leaf);
			}
			
		}
	}
	Root->CreateRooms(this, RoomActor);
	Leafs.Emplace();
}



void APG_MapGenerator::CorridorsMapCorridors(FVector RoomA, FVector RoomB)
{
	FRandomStream Stream;
	Stream.Initialize(200);
	//Get min and Max extents of each room representing outside corners 
	int32 A_X;
	int32 A_Y;
	int32 A_Z;
	int32 A_X2;
	int32 A_Y2;
	int32 B_X;
	int32 B_Y;
	int32 B_Z;
	int32 B_X2;
	int32 B_Y2;
	int32 Out_X;
	int32 Out_Y;
	FVector Point_A;
	FVector Point_B;
	FVector Point_Corner;
	TArray<FVector> Rooms;
	Rooms.Empty();
	TArray<FVector> Tiles;
	Tiles.Empty();
	//Get Corners
	A_X = RoomA.X;
	A_Y = RoomA.Y;
	A_Z = RoomA.Z;	int32 FindRoomAIndex = Rooms.Find(RoomA);
	A_X2 = Rooms[FindRoomAIndex].X;
	A_Y2 = Rooms[FindRoomAIndex].Y;
	B_X = RoomB.X;
	B_Y = RoomB.Y;
	B_Z = RoomB.Z;
	int32 FindRoomBIndex = Rooms.Find(RoomB);
	B_X2 = Rooms[FindRoomBIndex].X;;
	B_Y2 = Rooms[FindRoomBIndex].Y;;

	//Are rooms parallel on X with overlapping sides
	if(FMath::Max(A_X, B_X) <= FMath::Min(A_X2, B_X2))
	{
		//Room B is to the right?
		if(B_Y > A_Y)
		{
			//rooms are not merged?
			if(B_Y-A_Y2 > 1)
			{
				//Make corridor from A to B on Y axis 
				Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,FMath::Max(A_X, B_X), FMath::Min(A_X2, B_X2));
				Point_A = FVector(Out_X, A_Y2, A_Z);
				Point_B = FVector(Out_X, B_Y2, B_Z);
				Tiles = CorridorMakeY(Point_A, Point_B);
			}
		}
		else
		{
			//rooms are not merged?
			if(A_Y-B_Y2 > 1)
			{
				//Make corridor from B to A on Y axis 
				Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,FMath::Max(A_X, B_X), FMath::Min(A_X2, B_X2));
				Point_A = FVector(Out_X, A_Y, A_Z);
				Point_B = FVector(Out_X, B_Y2, B_Z);
				Tiles = CorridorMakeY(Point_B, Point_A);
			}
		}
	}
	else
	{
		//Are rooms parallel on X with overlapping sides
		if(FMath::Max(A_Y, B_Y) <= FMath::Min(A_Y2, B_Y2))
		{
			//Room B is to the Forward?
			if(B_X > A_X)
			{
				//rooms are not merged?
				if(B_X-A_X2 > 1)
				{
					//Make corridor from A to B on X axis 
					Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream, FMath::Max(A_Y, B_Y), FMath::Min(A_Y2, B_Y2));
					Point_A = FVector(A_X2, Out_Y, A_Z);
					Point_B = FVector(B_X, Out_Y, B_Z);
					Tiles = CorridorMakeX(Point_A, Point_B);
				}
			}
			else
			{
				//rooms are not merged?
				if(A_X-B_X2 > 1)
				{
					//Make corridor from B to A on X axis 
					Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream, FMath::Max(A_Y, B_Y), FMath::Min(A_Y2, B_Y2));
					Point_A = FVector(A_X, Out_Y, A_Z);
					Point_B = FVector(B_X2, Out_Y, B_Z);
					Tiles = CorridorMakeX(Point_B, Point_A);
				}
			}
		}
		else
		{
			//Room B is to the Forward?
			if(B_X > A_X)
			{
				if(B_Y > A_Y)
				{
					if(UKismetMathLibrary::RandomBoolFromStream(Stream))
					{
						//Make corridor from A to Corner (X), From Corner to B (Y). Hook up then Right
						Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,A_X, A_X2);
						Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,B_Y, B_Y2);
						Point_A = FVector(A_X2, Out_Y, A_Z);
						Point_B = FVector(Out_X, B_Y, B_Z);
						Point_Corner = FVector(Out_X, Out_Y, A_Z);
						Tiles.Append(CorridorMakeX(Point_A, Point_Corner));
						Tiles.Append(CorridorMakeX(Point_Corner, Point_B));
						Tiles.Add(Point_Corner);
					}
					else
					{
						//Make corridor from A to Corner (Y), From Corner to B (X). Hook Right then Up
						Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,B_X, B_X2);
						Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,A_Y, A_Y2);
						Point_A = FVector(Out_X, A_Y2, A_Z);
						Point_B = FVector(B_X, Out_Y, B_Z);
						Point_Corner = FVector(Out_X, Out_Y, A_Z);
						Tiles.Append(CorridorMakeX(Point_Corner, Point_B));
						Tiles.Append(CorridorMakeX(Point_A, Point_Corner));
						Tiles.Add(Point_Corner);
					}
				}
				else
				{
					if(UKismetMathLibrary::RandomBoolFromStream(Stream))
					{
						//Make corridor from A to Corner (X), From Corner to B (Y). Hook up then Left
						Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,B_X, B_X2);
						Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,A_Y, A_Y2);
						Point_A = FVector(A_X2, Out_Y, A_Z);
						Point_B = FVector(Out_X, B_Y2, B_Z);
						Point_Corner = FVector(Out_X, Out_Y, A_Z);
						Tiles.Append(CorridorMakeX(Point_A, Point_Corner));
						Tiles.Append(CorridorMakeX(Point_Corner, Point_B));
						Tiles.Add(Point_Corner);
					}
					else
					{
						//Make corridor from A to Corner (Y), From Corner to B (X). Hook Right then Up
						Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,A_X, A_X2);
						Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,B_Y, B_Y2);
						Point_A = FVector(Out_X, A_Y2, A_Z);
						Point_B = FVector(B_X2, Out_Y, B_Z);
						Point_Corner = FVector(Out_X, Out_Y, A_Z);
						Tiles = CorridorMakeX(Point_A, Point_B);
						Tiles.Append(CorridorMakeX(Point_A, Point_Corner));
						Tiles.Append(CorridorMakeX(Point_Corner, Point_B));
						Tiles.Add(Point_Corner);
					}
				} 
			}
			else
			{
				if(B_Y > A_Y)
				{
					if(UKismetMathLibrary::RandomBoolFromStream(Stream))
					{
						//Make corridor from A to Corner (Y), From Corner to B (X). Hook Right the up
						Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,A_X, A_X2);
						Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,B_Y, B_Y2);
						Point_A = FVector(A_X2, Out_Y, A_Z);
						Point_B = FVector(Out_X, B_Y, B_Z);
						Point_Corner = FVector(Out_X, Out_Y, A_Z);
						Tiles = CorridorMakeX(Point_A, Point_B);
						Tiles.Append(CorridorMakeX(Point_B, Point_Corner));
						Tiles.Append(CorridorMakeX(Point_A, Point_Corner));
						Tiles.Add(Point_Corner);
					}
					else
					{
						//Make corridor from Corner to A(Y), From Corner to B (Y). Hook Down then Right
						Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,B_X, B_X2);
						Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,A_Y, A_Y2);
						Point_A = FVector(A_X, Out_Y, A_Z);
						Point_B = FVector(Out_X, B_Y, B_Z);
						Point_Corner = FVector(Out_X, Out_Y, A_Z);
						Tiles.Append(CorridorMakeX(Point_B, Point_Corner));
						Tiles.Append(CorridorMakeX(Point_A, Point_Corner));
						Tiles.Add(Point_Corner);
					}
				}
				else
				{
					if(UKismetMathLibrary::RandomBoolFromStream(Stream))
					{
						//Make corridor from Corner to A (Y), From B to Corner (X). Hook up then Left
						Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,A_X, A_X2);
						Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,B_Y, B_Y2);
						Point_A = FVector(Out_X, A_Y, A_Z);
						Point_B = FVector(B_X2, Out_Y, B_Z);
						Point_Corner = FVector(Out_X, Out_Y, A_Z);
						Tiles.Append(CorridorMakeX(Point_B, Point_Corner));
						Tiles.Append(CorridorMakeX(Point_A, Point_Corner));
						Tiles.Add(Point_Corner);
					}
					else
					{
						//Make corridor from Corner to A (X), From B to Corner (X). Hook Down then Left
						Out_X = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,B_X, B_X2);
						Out_Y = UKismetMathLibrary::RandomIntegerInRangeFromStream(Stream,A_Y, A_Y2);
						Point_A = FVector(A_X, Out_Y, A_Z);
						Point_B = FVector(Out_X, B_Y2, B_Z);
						Point_Corner = FVector(Out_X, Out_Y, A_Z);
						Tiles.Append(CorridorMakeX(Point_B, Point_Corner));
						Tiles.Append(CorridorMakeX(Point_A, Point_Corner));
						Tiles.Add(Point_Corner);
					}
				} 
			}
		}
	}

	
}

TArray<FVector> APG_MapGenerator::CorridorMakeY(FVector From, FVector To)
{
	TArray<FVector> Tiles;
	Tiles.Empty();
	int32 Steps = 0;
	Steps = UKismetMathLibrary::Abs(From.Y - To.Y) - 1;
	
	for (int32 i = 1 ; i < Steps; ++i)
	{
		Tiles.Add(FVector( From.X, i + From.Y, From.Z));
	}
	return Tiles;
}

TArray<FVector> APG_MapGenerator::CorridorMakeX(FVector From, FVector To)
{
	TArray<FVector> Tiles;
	Tiles.Empty();
	int32 Steps = 0;
	Steps = UKismetMathLibrary::Abs(From.X - To.X) - 1;
	
	for (int32 i = 1 ; i < Steps; ++i)
	{
		Tiles.Add(FVector( i + From.X,  From.Y, From.Z));
	}
	return Tiles;
}
