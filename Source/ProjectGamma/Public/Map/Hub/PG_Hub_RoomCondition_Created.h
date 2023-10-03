// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Hub/PG_Hub_RoomGenerator.h"
#include "PG_Hub_RoomCondition_Created.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_Hub_RoomCondition_Created : public UPG_Hub_Fragment
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomCondition RoomCondition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomType RoomType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<APG_Hub_RoomActor> Room;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Supplies = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Power = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MonthlyCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	
	bool IsCurrentCondition(FRoomCondition RoomCondition);

	UFUNCTION(BlueprintCallable)
	virtual FRoomDescription GetDescription() const;
};
