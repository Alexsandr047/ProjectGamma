// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_Hub_RoomCondition_Created.h"
#include "PG_Hub_RoomCondition_Transitional.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_Hub_RoomCondition_Transitional : public UPG_Hub_RoomCondition_Created
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ConstructionDays = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPG_Hub_RoomCondition_Created> NextCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillId = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<APG_Hub_RoomActor> ActorForPreview;
	
	virtual FRoomDescription GetDescription() const override;
};
