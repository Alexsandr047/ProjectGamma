// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_RoomGenerator.h"
#include "Engine/DataAsset.h"
#include "PG_Base_RoomList.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_Base_RoomList : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPG_Base_RoomList(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditDefaultsOnly)
	float RoomsSize;

	UPROPERTY(EditDefaultsOnly, Category =  "Base Rooms")
	TArray<FRoom_Data> OneDoor;

	UPROPERTY(EditDefaultsOnly, Category =  "Base Rooms")
	TArray<FRoom_Data> TwoDoorsStraight;
	
	UPROPERTY(EditDefaultsOnly, Category =  "Base Rooms")
	TArray<FRoom_Data> TwoDoorsAngle;
	
	UPROPERTY(EditDefaultsOnly, Category =  "Base Rooms")
	TArray<FRoom_Data> ThreeDoors;
	
	UPROPERTY(EditDefaultsOnly, Category =  "Base Rooms")
	TArray<FRoom_Data> FourDoors;

	UPROPERTY(EditDefaultsOnly, Category =  "Start Rooms")
	TArray<FRoom_Data> Start_OneDoor;

	UPROPERTY(EditDefaultsOnly, Category =  "Start Rooms")
	TArray<FRoom_Data> Start_TwoDoorsStraight;

	UPROPERTY(EditDefaultsOnly, Category =  "Start Rooms")
	TArray<FRoom_Data> Start_TwoDoorsAngle;

	UPROPERTY(EditDefaultsOnly, Category =  "Start Rooms")
	TArray<FRoom_Data> Start_ThreeDoors;

	UPROPERTY(EditDefaultsOnly, Category =  "Start Rooms")
	TArray<FRoom_Data> Start_FourDoors;

	UPROPERTY(EditDefaultsOnly, Category =  "Boss Rooms")
	TArray<FRoom_Data> BossRoms;

	UPROPERTY(EditDefaultsOnly, Category =  "Finish Rooms")
	TArray<FRoom_Data> FinishRooms;

	UPROPERTY(EditDefaultsOnly, Category =  "Secret Rooms")
	TArray<FRoom_Data> SecretRooms;

	TArray<FRoom_Data> GetOneDoorRooms(int32 Type) const;
	TArray<FRoom_Data> GetTwoDoorsStraightRooms(int32 Type) const;
	TArray<FRoom_Data> GetTwoDoorsAngleRooms(int32 Type) const;
	TArray<FRoom_Data> GetThreeDoorsRooms(int32 Type) const;
	TArray<FRoom_Data> GetFourDoorsRooms(int32 Type) const;
};
