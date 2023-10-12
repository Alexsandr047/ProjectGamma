// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_Hub_RoomActor.h"
#include "Core/PG_CoreTypes.h"

#include "GameFramework/Actor.h"
#include "PG_Hub_MapGenerator.generated.h"

class UPG_Skill_Tree;
class UPG_Hub_Fragment;
class APG_Hub_RoomGenerator;

USTRUCT(BlueprintType)
struct FRoomCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomCondition RoomCondition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomType RoomType;

	FRoomCondition()
	{
		RoomCondition = ERoomCondition::Debris;
		RoomType = ERoomType::Debris;
	}
	FRoomCondition(ERoomCondition _RoomCondition, ERoomType _RoomType)
	{
		RoomCondition = _RoomCondition;
		RoomType = _RoomType;
	}

	bool operator==(const FRoomCondition& other) const
	{
		if(this->RoomCondition == other.RoomCondition && this->RoomType == other.RoomType)
			return true;

		return false;
	}
};

USTRUCT(BlueprintType)
struct FRoomFragments
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPG_Hub_Fragment> Condition;
};

USTRUCT(BlueprintType)
struct FRoomData: public FRoomFragments
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
		
};

USTRUCT(BlueprintType)
struct FRoomDescription
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Supplies = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Power = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuildTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MonthlyCost = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillId = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<APG_Hub_RoomActor> ActorForPreview = nullptr;

	UPROPERTY()
	FRoomCondition RoomCondition;
	
	FRoomDescription(){};

	FRoomDescription(FText _Title, int32 _Supplies, int32 _Power, int32 _MonthlyCost,
		FText _Description, FRoomCondition _RoomCondition)
	{
		Title = _Title;
		Supplies = _Supplies;
		Power = _Power;
		MonthlyCost = _MonthlyCost;
		Description = _Description;
		ActorForPreview = nullptr;
		RoomCondition = _RoomCondition;
	}
	FRoomDescription(FText _Title,int32 _Supplies, int32 _Power, int32 _BuildTime, int32 _MonthlyCost,
	FString _SkillId,	FText _Description, TSoftClassPtr<APG_Hub_RoomActor> _ActorForPreview, FRoomCondition _RoomCondition)
	{
		Title = _Title;
		Supplies = _Supplies;
		Power = _Power;
		BuildTime = _BuildTime;
		MonthlyCost = _MonthlyCost;
		SkillId = _SkillId;
		Description = _Description;
		ActorForPreview = _ActorForPreview;
		RoomCondition = _RoomCondition;
	}
};

USTRUCT()
struct FShipData
{
	GENERATED_BODY()
	
	FDateTime DateTo;
	
	TArray<FShipRoomBase> ShipRooms;

	TArray<FSkillParseData> Technologies;
	
	TArray<FShipResourceBase> ShipResources;

	TArray<FSquad> Squads;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdate, const  FDateTime&, DateTime);

UCLASS()
class PROJECTGAMMA_API APG_Hub_MapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_Hub_MapGenerator();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectGamma|Rooms Type")
	TArray<FRoomData> Rooms;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectGamma|Rooms Type")
	TArray<TSubclassOf<UPG_Hub_Fragment>> RoomConditions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectGamma|Room Generator")
	TSubclassOf<APG_Hub_RoomGenerator> RoomGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectGamma|Room Generator")
	TSubclassOf<UPG_Skill_Tree> TechTree;
	
	void SetDateTo(FDateTime DateTo);
	void GenerateRooms(TArray<FShipRoomBase> ShipRooms);
	void InitTechTree(TArray<FSkillParseData> Technologies);
	TSubclassOf<UPG_Hub_Fragment> FindRoomConditions(FRoomCondition RoomCondition);
	
	UPROPERTY()
	TArray<FRoomData> RoomWaitingList;
	
	UFUNCTION()
	void UpdateList();
	void UpdateList(FRoomData NewRoom);
	
	//Time Manager

	/* Handle to manage the timer */
	FTimerHandle TimeTimerHandle;

	float TimeRate = 1.f;
	
	UPROPERTY()
	bool bStartTime = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectGamma|Rooms Type")
	FDateTime DateTo;

protected:
	UPROPERTY()
	TMap<FString, int32> ShipResources;
public:
	UFUNCTION(BlueprintCallable)
	int32 GetShipResourcesById(const FString& ResourceID) const;
	UFUNCTION(BlueprintCallable)
	void AddShipResources(const FString& ResourceID, int32 Quantity);
	void SetShipResources(const FString& ResourceID, int32 Quantity);
	TMap<FString, int32> GetShipResources() const { return ShipResources;  }
	
	UFUNCTION(BlueprintCallable)
	void StartTime();
	UFUNCTION(BlueprintCallable)
	void StopTime();	
	UFUNCTION()
	void CalculateTime();	
	UPROPERTY(BlueprintAssignable, Category = "ProjectGamma|Time")
	FOnTimeUpdate TimeUpdate;	
	UFUNCTION(BlueprintCallable)
	void UpdateData();
	UFUNCTION()
	void AddRoomForCheck(APG_Hub_RoomGenerator* const _RoomGenerator);
	void CheckRoomChanges();
	
	UFUNCTION(BlueprintCallable)
	TArray<APG_Hub_RoomGenerator*> GetRoomGenerators() { return RoomGenerators; }
	UFUNCTION(BlueprintCallable)
	TArray<FRoomDescription> GetRoomsCheckList();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual  void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	TArray<APG_Hub_RoomGenerator*> ChangingRooms;
	
	UPROPERTY()
	TArray<APG_Hub_RoomGenerator*> RoomGenerators;

};
