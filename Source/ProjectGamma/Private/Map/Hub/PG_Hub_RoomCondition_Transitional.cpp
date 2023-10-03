// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Hub/PG_Hub_RoomCondition_Transitional.h"

FRoomDescription UPG_Hub_RoomCondition_Transitional::GetDescription() const
{
	return FRoomDescription(Title, Supplies, Power, ConstructionDays, MonthlyCost, SkillId, Description, ActorForPreview, FRoomCondition(RoomCondition, RoomType));
}
