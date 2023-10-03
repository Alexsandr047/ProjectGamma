// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_Hub_MapGenerator.h"
#include "Core/PG_CoreTypes.h"
#include "GameFramework/Actor.h"
#include "PG_Hub_RoomGenerator.generated.h"

class APG_Hub_RoomGenerator;
class APG_Hub_RoomActor;
class UPG_Hub_Params_Fragment;
class UPG_Hub_RoomCondition_Created;

UCLASS(BlueprintType, Blueprintable, Abstract)
class PROJECTGAMMA_API UPG_Hub_Fragment : public UObject
{
	GENERATED_BODY()

public:
	UPG_Hub_Fragment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerUpdate, const  FTimespan&, DateTime);

UCLASS()
class PROJECTGAMMA_API APG_Hub_RoomGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APG_Hub_RoomGenerator();
	
	UPROPERTY()
	APG_Hub_RoomActor* CurrentRoom;
	
	UPROPERTY()
	APG_Hub_MapGenerator* MapGenerator;
	
	void SetMapGenerator(APG_Hub_MapGenerator* MapGenerator);
	
	void CreateRoom(const FRoomData* CoreData);
	void CreateRoomFromParsedData(const FRoomData* CoreData, const FShipRoomBase* ParsedDara);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetRoomData(const APG_Hub_RoomGenerator* RoomGenerator);

	UFUNCTION(BlueprintCallable, meta =(ExpandEnumAsExecs="ReturnValue"))
	ERoomCondition GetRoomCondition();

	UFUNCTION(BlueprintCallable, meta =(ExpandEnumAsExecs="ReturnValue"))
	ERoomType GetRoomType();

	UFUNCTION(BlueprintCallable, meta =(ExpandBoolAsExecs = "ReturnValue"))
	bool GetTransitionalRoomData( int32& Days, ERoomType& NextCondition);

	UFUNCTION(BlueprintCallable, meta =(ExpandBoolAsExecs = "ReturnValue"))
	bool StartClearingRoom();

	UPROPERTY(BlueprintAssignable, Category = "ProjectGamma|Time")
	FOnTimerUpdate TimeUpdate;
	
	bool FinishChanging();

	FShipRoomBase GetRoomData();

	UFUNCTION(BlueprintCallable)
	void UpdateTime();

	FRoomDescription GetRoomDescription() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	FShipRoomBase RoomData;
	
	UPROPERTY()
	FDateTime FinishDay;

	bool StartChanging();	
	
	UPROPERTY()
	APG_Hub_RoomActor* RoomActor;
public:

	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category=Display, Instanced)
	UPG_Hub_RoomCondition_Created* Condition;

	/*UFUNCTION(BlueprintCallable)
	UPG_Hub_Fragment* FindFragmentByClass(TSubclassOf<UPG_Hub_Fragment> FragmentClass) const;*/
	
};
