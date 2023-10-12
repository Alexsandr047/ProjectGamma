// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Hub/PG_Hub_MapGenerator.h"

#include "Character/PG_Skill_Subsystem.h"
#include "InventorySystem/PG_PlayerDataSubsystem.h"
#include "Map/Hub/PG_Hub_RoomCondition_Created.h"
#include "Map/Hub/PG_Hub_RoomGenerator.h"


void APG_Hub_MapGenerator::SetDateTo(FDateTime _DateTo)
{
	DateTo = _DateTo;
}

void APG_Hub_MapGenerator::GenerateRooms(TArray<FShipRoomBase> ShipRooms)
{	
	if(Rooms.Num()> 0)
	{
		for (auto& Room : Rooms)
		{
			if(Room.ID == "")
				return;
			
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APG_Hub_RoomGenerator* RG = GetWorld()->SpawnActor<APG_Hub_RoomGenerator>(RoomGenerator, Room.Location, FRotator (0.f), SpawnParams);
			if(RG)
			{
				bool IdFound = false;
				RG->SetMapGenerator(this);
				RoomGenerators.Add(RG);
				
				for(auto& ShipRoom : ShipRooms)
				{
					if(ShipRoom.Id == Room.ID)
					{
						IdFound = true;
						RG->CreateRoomFromParsedData(&Room, &ShipRoom);
					}
				}
				if(!IdFound)
					RG->CreateRoom(&Room);				
			}
		}
	}
}

void APG_Hub_MapGenerator::InitTechTree(TArray<FSkillParseData> Technologies)
{
	UGameInstance* GI = GetGameInstance();
	UPG_Skill_Subsystem* Skill_Subsystem = GI->GetSubsystem<UPG_Skill_Subsystem>();
	check(Skill_Subsystem);

	Skill_Subsystem->InitSkillTree("TechTree", Technologies, TechTree);
}

TSubclassOf<UPG_Hub_Fragment> APG_Hub_MapGenerator::FindRoomConditions(FRoomCondition RoomCondition)
{
	TSubclassOf<UPG_Hub_Fragment> Fragment;

	for(auto& RC : RoomConditions)
	{
		if(UPG_Hub_RoomCondition_Created* ConditionFragment = Cast<UPG_Hub_RoomCondition_Created>(RC.Get()->ClassDefaultObject))
			if(ConditionFragment->IsCurrentCondition(RoomCondition))
				return RC;
	}
	
	return Fragment;
}

void APG_Hub_MapGenerator::UpdateList()
{
	
}

void APG_Hub_MapGenerator::UpdateList(FRoomData NewRoom)
{
	RoomWaitingList.Add(NewRoom);
}

int32 APG_Hub_MapGenerator::GetShipResourcesById(const FString& ResourceID) const
{
	if(ShipResources.Find(ResourceID))
		return *ShipResources.Find(ResourceID);
	return -1;
}

void APG_Hub_MapGenerator::AddShipResources(const FString& ResourceID, int32 Quantity)
{
	if(ShipResources.Find(ResourceID))
	{
		int32 Val = *ShipResources.Find(ResourceID) + Quantity;
		ShipResources.Emplace(ResourceID,Val);		
	}
	else
		ShipResources.Add(*ResourceID, Quantity);
}

void APG_Hub_MapGenerator::SetShipResources(const FString& ResourceID, int32 Quantity)
{
	ShipResources.Emplace(ResourceID,Quantity);
}

void APG_Hub_MapGenerator::StartTime()
{
	bStartTime = true;
	CalculateTime();
	UpdateData();
}

void APG_Hub_MapGenerator::StopTime()
{
	bStartTime = false;
	GetWorld()->GetTimerManager().ClearTimer(TimeTimerHandle);
	UpdateData();
}

void APG_Hub_MapGenerator::CalculateTime()
{
	if(!bStartTime)
		return;
	
	FTimespan Second(0,1,0,0,0);
	
	DateTo = DateTo + Second;

	CheckRoomChanges();
	
	TimeUpdate.Broadcast(DateTo);
	/* Activate the fuze to explode the bomb after several seconds */
	GetWorld()->GetTimerManager().SetTimer(TimeTimerHandle, this, &APG_Hub_MapGenerator::CalculateTime, TimeRate, false);
}

void APG_Hub_MapGenerator::CheckRoomChanges()
{
	auto NewChangingRooms = ChangingRooms;
	for (auto& Room : NewChangingRooms)
	{
		if(Room->FinishChanging())
			ChangingRooms.Remove(Room);
	}
}

void APG_Hub_MapGenerator::AddRoomForCheck(APG_Hub_RoomGenerator* const _RoomGenerator)
{
	ChangingRooms.Add(_RoomGenerator);
}

void APG_Hub_MapGenerator::UpdateData()
{
	UGameInstance* GI = GetGameInstance();

	UPG_PlayerDataSubsystem* PlayerDataSubsystem = GI->GetSubsystem<UPG_PlayerDataSubsystem>();
	check(PlayerDataSubsystem);

	PlayerDataSubsystem->WriteJsonFileAboutShipData(nullptr,this);
}

TArray<FRoomDescription> APG_Hub_MapGenerator::GetRoomsCheckList()
{
	TArray<FRoomDescription> CheckList;
	
	UGameInstance* GI = GetGameInstance();
	UPG_Skill_Subsystem* SkillSubsystem =  GI->GetSubsystem<UPG_Skill_Subsystem>();
	check(SkillSubsystem);

	if(UPG_Skill_Tree* Skill_Tree = SkillSubsystem->GetSkillTreeById("TechTree"))
	{
		TMap<ERoomType, FRoomDescription> RoomsСanBuild;
		for(auto& RC : RoomConditions)
		{
			if(UPG_Hub_RoomCondition_Created* ConditionFragment = Cast<UPG_Hub_RoomCondition_Created>(RC.Get()->ClassDefaultObject))
				if(ERoomType::Debris != ConditionFragment->RoomType && ERoomType::Empty != ConditionFragment->RoomType && ERoomCondition::Building == ConditionFragment->RoomCondition)
					if(ESkillStatus::Lock != Skill_Tree->CheckStatusSkill(ConditionFragment->GetDescription().SkillId))
						RoomsСanBuild.Add(ConditionFragment->RoomType,ConditionFragment->GetDescription());
		}
		for(auto& RG: RoomGenerators)
		{
			if(ERoomType::Debris != RG->GetRoomType() && ERoomType::Empty != RG->GetRoomType() && ERoomCondition::Building != RG->GetRoomCondition())
				RoomsСanBuild.Remove(RG->GetRoomType());
		}
		RoomsСanBuild.GenerateValueArray(CheckList);
	}	
	return CheckList;
}

// Called when the game starts or when spawned
void APG_Hub_MapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void APG_Hub_MapGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Ensure the fuze timer is cleared by using the timer handle
	GetWorld()->GetTimerManager().ClearTimer(TimeTimerHandle);

	// Alternatively you can clear ALL timers that belong to this (Actor) instance.
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


