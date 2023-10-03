// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/PG_Base_RoomList.h"

UPG_Base_RoomList::UPG_Base_RoomList(const FObjectInitializer& ObjectInitializer)
{
	
}

TArray<FRoom_Data> UPG_Base_RoomList::GetOneDoorRooms(int32 Type) const
{
	if(Type == 1)
		return Start_OneDoor;
	if(Type == 2)
		return BossRoms;
	if(Type == 3)
		return FinishRooms;
	if(Type ==4)
		return SecretRooms;
	return OneDoor;
}

TArray<FRoom_Data> UPG_Base_RoomList::GetTwoDoorsStraightRooms(int32 Type) const
{
	if(Type == 1)
		return  Start_TwoDoorsStraight;
	return TwoDoorsStraight;
}

TArray<FRoom_Data> UPG_Base_RoomList::GetTwoDoorsAngleRooms(int32 Type) const
{
	if(Type == 1)
		return  Start_TwoDoorsAngle;
	return TwoDoorsAngle;
}

TArray<FRoom_Data> UPG_Base_RoomList::GetThreeDoorsRooms(int32 Type) const
{
	if(Type == 1)
		return  Start_ThreeDoors;
	return  ThreeDoors;
}

TArray<FRoom_Data> UPG_Base_RoomList::GetFourDoorsRooms(int32 Type) const
{
	if(Type == 1)
		return  Start_FourDoors;
	return FourDoors;
}
