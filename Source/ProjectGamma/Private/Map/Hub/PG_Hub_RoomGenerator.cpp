// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Hub/PG_Hub_RoomGenerator.h"

#include "Core/CIS_UtilityFunctionLibrary.h"
#include "InventorySystem/PG_PlayerDataSubsystem.h"
#include "Map/Hub/PG_Hub_RoomActor.h"
#include "Map/Hub/PG_Hub_RoomCondition_Created.h"
#include "Map/Hub/PG_Hub_RoomCondition_Transitional.h"


UPG_Hub_Fragment::UPG_Hub_Fragment(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Sets default values
APG_Hub_MapGenerator::APG_Hub_MapGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipResources.Add("Money", 100);
	ShipResources.Add("Intel", 100);
}



// Sets default values
APG_Hub_RoomGenerator::APG_Hub_RoomGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

void APG_Hub_RoomGenerator::SetMapGenerator(APG_Hub_MapGenerator* MG)
{
	MapGenerator = MG;
}

void APG_Hub_RoomGenerator::CreateRoom(const FRoomData*_RoomData)
{
	if(_RoomData->ID != "")
	{
		RoomData.Id = _RoomData->ID;
		FAsyncLoadClassObjectDelegate* OnAsyncLoadOptionsRoomCompleteDelagate = new FAsyncLoadClassObjectDelegate();
		TArray<TSoftClassPtr<UObject>> RoomsToLoad;
		if(UPG_Hub_RoomCondition_Created* RoomCondition = NewObject<UPG_Hub_RoomCondition_Created>(this, _RoomData->Condition))
		{
			RoomsToLoad.Add(RoomCondition->Room);
			Condition = RoomCondition;
			SetRoomData(this);
			OnAsyncLoadOptionsRoomCompleteDelagate->BindLambda([this](const TArray<TSubclassOf<UObject>>& Rooms)
			{
				if(Rooms[0])
				{
			
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					APG_Hub_RoomActor* RA =GetWorld()->SpawnActor<APG_Hub_RoomActor>(Rooms[0].Get(), GetActorLocation(), FRotator (0.f), SpawnParams);
					if(RA)
					{
						if(RoomActor)
							RoomActor->Destroy();

						RoomActor = RA;
					}		
				}
			});
		UCIS_UtilityFunctionLibrary::AsyncLoadClassObject(GetWorld(),RoomsToLoad, OnAsyncLoadOptionsRoomCompleteDelagate);
		}
	}
}

void APG_Hub_RoomGenerator::CreateRoomFromParsedData(const FRoomData* CoreData, const FShipRoomBase* ParsedDara)
{
	if(!MapGenerator)
		return;
	
	if(TSubclassOf<UPG_Hub_Fragment> Hub_Fragment = *MapGenerator->FindRoomConditions(FRoomCondition(ParsedDara->RoomCondition,ParsedDara->RoomType)))
	{
		FRoomData CD;
		CD = *CoreData;
		CD.Condition = Hub_Fragment;
		FDateTime::Parse(ParsedDara->FinishDay,FinishDay);
		CreateRoom(&CD);
		if(ParsedDara->RoomCondition == ERoomCondition::Building || ParsedDara->RoomCondition == ERoomCondition::Clearing)
			MapGenerator->AddRoomForCheck(this);
	}
	
}

ERoomCondition APG_Hub_RoomGenerator::GetRoomCondition()
{
	if(Condition)
		return Condition->RoomCondition;
	else
		return ERoomCondition::Debris;
}

ERoomType APG_Hub_RoomGenerator::GetRoomType()
{
	if(Condition)
		return Condition->RoomType;
	return ERoomType::Debris;
}

bool APG_Hub_RoomGenerator::GetTransitionalRoomData(int32& Days, ERoomType& NextCondition)
{
	if(UPG_Hub_RoomCondition_Transitional* TC = Cast<UPG_Hub_RoomCondition_Transitional>(Condition))
	{
		//Days = TC->ConstructionDays;
		if(UPG_Hub_RoomCondition_Transitional* NextTC = Cast<UPG_Hub_RoomCondition_Transitional>(TC->NextCondition.Get()->ClassDefaultObject))
		{
			Days = NextTC->ConstructionDays;
			NextCondition = NextTC->RoomType;
			return true;
		}
		return false;
	}
	return false;
}

bool APG_Hub_RoomGenerator::StartClearingRoom()
{
	return StartChanging();
}

// Called when the game starts or when spawned
void APG_Hub_RoomGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

bool APG_Hub_RoomGenerator::StartChanging()
{
	if(MapGenerator)
	{
		if(UPG_Hub_RoomCondition_Transitional* TC = Cast<UPG_Hub_RoomCondition_Transitional>(Condition))
		{
			if(UPG_Hub_RoomCondition_Transitional* NextTC = Cast<UPG_Hub_RoomCondition_Transitional>(TC->NextCondition.Get()->ClassDefaultObject))
			{
				FDateTime DateTo = MapGenerator->DateTo;
				FTimespan Days(NextTC->ConstructionDays,0,0,0,0);
				FinishDay = DateTo + Days;
				MapGenerator->AddRoomForCheck(this);
				FRoomData RD;
				RD.ID = RoomData.Id;
				RD.Location = GetActorLocation();
				RD.Condition = TC->NextCondition;
				CreateRoom(&RD);
				TimeUpdate.Broadcast(Days);
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

bool APG_Hub_RoomGenerator::FinishChanging()
{
	if(MapGenerator)
	{
		if(UPG_Hub_RoomCondition_Transitional* TC = Cast<UPG_Hub_RoomCondition_Transitional>(Condition))
		{			
			if(FinishDay <= MapGenerator->DateTo)
			{
				//FinishDay = FDateTime(0,0,0,0,0,0,0);
				FRoomData RD;
				RD.ID = RoomData.Id;
				RD.Location = GetActorLocation();
				RD.Condition = TC->NextCondition;
				CreateRoom(&RD);
				return true;
			}
			TimeUpdate.Broadcast(FinishDay - MapGenerator->DateTo);
			return false;		
		}
		return false;
	}
	return false;
}

FShipRoomBase APG_Hub_RoomGenerator::GetRoomData()
{
	RoomData.FinishDay = FinishDay.ToString();
	if(UPG_Hub_RoomCondition_Created* RC = Cast<UPG_Hub_RoomCondition_Created>(Condition))
	{
		RoomData.RoomCondition = RC->RoomCondition;
		RoomData.RoomType = RC->RoomType;
	}
	return RoomData;
}

void APG_Hub_RoomGenerator::UpdateTime()
{
	if(MapGenerator)
		TimeUpdate.Broadcast(FinishDay - MapGenerator->DateTo);
}

FRoomDescription APG_Hub_RoomGenerator::GetRoomDescription() const
{
	if(Condition)
		return Condition->GetDescription();
	return FRoomDescription();
}

