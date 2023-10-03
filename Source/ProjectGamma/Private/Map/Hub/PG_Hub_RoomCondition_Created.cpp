// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Hub/PG_Hub_RoomCondition_Created.h"

bool UPG_Hub_RoomCondition_Created::IsCurrentCondition(FRoomCondition _RoomCondition)
{
	if(_RoomCondition.RoomCondition == RoomCondition && _RoomCondition.RoomType == RoomType)
		return true;
	
	return false;
}

FRoomDescription UPG_Hub_RoomCondition_Created::GetDescription() const
{
	return FRoomDescription(Title, Supplies, Power, MonthlyCost, Description, FRoomCondition(RoomCondition, RoomType));
}
