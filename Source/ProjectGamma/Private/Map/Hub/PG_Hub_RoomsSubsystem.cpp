// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Hub/PG_Hub_RoomsSubsystem.h"
#include "Core/PG_CoreTypes.h"
#include "JsonObjectConverter.h"
#include "Character/PG_Skill_Subsystem.h"
#include "Core/PG_GameInstance.h"
#include "Map/Hub/PG_Hub_MapGenerator.h"
#include "Map/Hub/PG_Hub_RoomGenerator.h"

void UPG_Hub_RoomsSubsystem::ReadJsonFileAndMakeShipData(APG_ShipBasePlayerController* PlayerController,
                                                         FOnShipDataParsed ItemParsed)
{
	UPG_GameInstance* GI = Cast<UPG_GameInstance>(GetGameInstance());
	if(!GI)
	check(PlayerController);
	
	const FString JsonFilePath = FPaths::ProjectSavedDir() + "JsonFiles/"+ GI->GetPlayerId() +"/ShipData.json";
	FString JsonString; //Json converted to FString
	
	FShipData ShipData;
	
	FFileHelper::LoadFileToString(JsonString,*JsonFilePath);
	
	//Displaying the json in a string format inside the output log
	GLog->Log("Json String:");
	GLog->Log(JsonString);
	const TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonValue> JsonValue;	
	if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
	{
		const TSharedPtr<FJsonObject>* JsonObject;
		JsonValue->TryGetObject(JsonObject);

		FString DateTo = "";
		JsonObject->Get()->TryGetStringField("DateTo", DateTo);
		FDateTime::Parse(DateTo, ShipData.DateTo);

		const TArray<TSharedPtr<FJsonValue>>* RoomsJsonData;
		JsonObject->Get()->TryGetArrayField("Rooms",RoomsJsonData);
		for (const auto& RoomValue : *RoomsJsonData)
		{
			const TSharedPtr<FJsonObject>* RoomValueObject;
			if(RoomValue->TryGetObject(RoomValueObject))
			{
				FShipRoomBase RoomData;
				if(FJsonObjectConverter::JsonObjectToUStruct<FShipRoomBase>(RoomValueObject->ToSharedRef(), &RoomData, 0, 0))
					ShipData.ShipRooms.Add(RoomData);
			}
		}
	}
	ItemParsed.Broadcast(PlayerController, ShipData);
}

void UPG_Hub_RoomsSubsystem::WriteJsonFileAboutShipData(APG_ShipBasePlayerController* PlayerController,
	APG_Hub_MapGenerator* MapGenerator)
{
	const FString JsonFilePath = FPaths::ProjectSavedDir() + "JsonFiles/ShipData.json";
	FString JsonString;

	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject);
	JsonObj->SetStringField("Player", "TestID");
	JsonObj->SetStringField("DateTo", MapGenerator->DateTo.ToString());

	TArray<TSharedPtr<FJsonValue> > RoomsObjArray;	
	for(auto& RoomGenerator : MapGenerator->GetRoomGenerators())
	{
		if(!RoomGenerator)
			return;
		
		TSharedPtr<FJsonObject> TestStructJsonObject = FJsonObjectConverter::UStructToJsonObject<FShipRoomBase>(RoomGenerator->GetRoomData());
		TSharedRef<FJsonValueObject> JsonValue = MakeShareable( new FJsonValueObject( TestStructJsonObject) );
		
		RoomsObjArray.Add(JsonValue);
	}	
	JsonObj->SetArrayField("Rooms", RoomsObjArray);

	TArray<TSharedPtr<FJsonValue> > TreeObjArray;
	UGameInstance* GI = GetGameInstance();
	UPG_Skill_Subsystem* Skill_Subsystem = GI->GetSubsystem<UPG_Skill_Subsystem>();
	check(Skill_Subsystem);

	if(UPG_Skill_Tree* Skill_Tree = Skill_Subsystem->GetSkillTreeById("TechTree"))
	{
		for(auto& TechSkill : Skill_Tree->GetSkillsData())
		{
			if(!TechSkill.IsValid())
				return;
			
			TSharedPtr<FJsonObject> TestStructJsonObject = FJsonObjectConverter::UStructToJsonObject<FSkillParseData>(TechSkill);
			TSharedRef<FJsonValueObject> JsonValue = MakeShareable( new FJsonValueObject( TestStructJsonObject) );
		
			TreeObjArray.Add(JsonValue);
		}
	}
	JsonObj->SetArrayField("TechTree", TreeObjArray);

	TArray<TSharedPtr<FJsonValue> > ResourcesObjArray;
	for(auto& Resource : MapGenerator->GetShipResources())
	{
		TSharedPtr<FJsonObject> TestStructJsonObject = FJsonObjectConverter::UStructToJsonObject<FShipResourceBase>(FShipResourceBase(Resource.Key, Resource.Value));
		TSharedRef<FJsonValueObject> JsonValue = MakeShareable( new FJsonValueObject( TestStructJsonObject) );
		ResourcesObjArray.Add(JsonValue);
	}
	JsonObj->SetArrayField("Resources", ResourcesObjArray);
	
	//JsonObj->SetObjectField(FString::FromInt(111), TestStructJsonObject);


	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);
	FFileHelper::SaveStringToFile(JsonString,*JsonFilePath);
}
