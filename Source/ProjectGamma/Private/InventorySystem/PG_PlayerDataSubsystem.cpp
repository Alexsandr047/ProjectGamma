// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/PG_PlayerDataSubsystem.h"

#include "JsonObjectConverter.h"
#include "Character/PG_Skill_Subsystem.h"
#include "Core/CIS_CoreTypes.h"
#include "Core/PG_AssetManager.h"
#include "Core/PG_CoreTypes.h"
#include "Core/PG_GameInstance.h"
#include "Core/PG_UtilityFunctionLibrary.h"
#include "InventorySystem/PG_QuickBarComponent.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_EquipmentItem.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_SetStats.h"
#include "InventorySystem/ItemsData/PG_Character_PlayerData.h"
#include "InventorySystem/ItemsData/PG_Ship_PlayerData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Map/Hub/PG_Hub_RoomGenerator.h"
#include "Player/PG_PlayerController.h"


UPG_GameInstance* UPG_PlayerDataSubsystem::GetGameGameInstance()
{
	if(!GameInstance)
		GameInstance = Cast<UPG_GameInstance>(GetGameInstance());
	return GameInstance;
}

FString UPG_PlayerDataSubsystem::GetPlayerID()
{	
	return  GetGameGameInstance()->GetPlayerId();
}

void UPG_PlayerDataSubsystem::ParsePlayerData(UPG_QuickBarComponent* QuickBarComponent, FOnItemParsed ItemParsed, FString PlayerData)
{
	check(QuickBarComponent);
		
	UPG_PlayerData* Player_Data = nullptr;
		
	if(QuickBarComponent->PlayerTypeData ==  EPlayerDataType::Commander)
	{
		const TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(PlayerData);
		TSharedPtr<FJsonValue> JsonValue;	
		if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
		{
			const TSharedPtr<FJsonObject>* JsonObject;
			JsonValue->TryGetObject(JsonObject);
		
			Player_Data = NewObject<UPG_Character_PlayerData>(this);
			const TSharedPtr<FJsonObject>* CommanderObject;
			JsonObject->Get()->TryGetObjectField("Commander", CommanderObject);
			ParseCharacterItem(CommanderObject, Cast<UPG_Character_PlayerData>(Player_Data));
			QuickBarComponent->SetParsedPlayerData(Player_Data);
		}
		else
			GLog->Log("couldn't deserialize");
	}
	if(QuickBarComponent->PlayerTypeData ==  EPlayerDataType::ShipBase)
	{
		Player_Data = NewObject<UPG_Ship_PlayerData>(this);
		ReadJsonFileAndMakeShipData(PlayerData,Cast<UPG_Ship_PlayerData>(Player_Data));
	}
	ItemParsed.Broadcast(QuickBarComponent, Player_Data);
	ItemParsed.Clear();
}

void UPG_PlayerDataSubsystem::SaveJsonFile()
{
	/*TArray<FUnmodifiableItemBase> Attachments;
	FUnmodifiableItemBase Scope;
	Scope.Id = "Scope";
	FUnmodifiableItemBase Barrel;
	Barrel.Id = "Barrel"; 
	Attachments.Add(Scope);
	Attachments.Add(Barrel);
	
	TArray<FModifiableItemBase> Itemas;		
	FModifiableItemBase Item;
	
	Item.Id = "DC15";
	Item.Attachments = Attachments;

	Itemas.Add(Item);*/
	
	/*TArray<TSharedPtr<FJsonValue> > ObjArray;
	TSharedPtr< FJsonObject > JsonObj = MakeShareable(new FJsonObject);
	JsonObj->SetStringField("Abc", "AbcValue");
	
	FJsonObjectConverter::UStructToJsonObject(Item,JsonObj, 0, 0);
	TSharedRef< FJsonValueObject > JsonValue = MakeShareable( new FJsonValueObject( JsonObj) );
	ObjArray.Add(JsonValue);
	
	JsonObj->SetArrayField("Item", ObjArray);*/
	
	/*ASRBattlePlayerState* ResultPlayerState = nullptr;
    
    	TArray<AController*> Controllers;
    	Controllers = ConnectedPlayers;
    	for (auto Player : UsedAI)
    	{
    		Controllers.Add(Player);
    	}
    	
    	for (auto Player : Controllers)
    	{
    		if (!Player)
    			continue;
    		
    		ASRBattlePlayerState* PlayerState = Player->GetPlayerState<ASRBattlePlayerState>();
    		const FString CurrentPlayerID = PlayerState->NakamaPlayerID;
    
    		if(CurrentPlayerID == PlayerID)
    		{
    			ResultPlayerState = PlayerState;
    			break;
    		}
    	}
    
    	if (!ResultPlayerState)
    	{
    		FBattleAnalytics EmptyBattleAnalytics;
    		EmptyBattleAnalytics.disconnect = true;
    		TSharedPtr<FJsonObject> AnalyticsJsonData = FJsonObjectConverter::UStructToJsonObject<FBattleAnalytics>(EmptyBattleAnalytics);
    	
    		return AnalyticsJsonData;
    	}
    	
    	FBattleAnalytics& BattleAnalytics = ResultPlayerState->Analytics;  
    	
    	BattleAnalytics.afk = ResultPlayerState->bPlayerInAFK;	
    	auto PlayerShip = Cast<ASRBattleSpaceShip>(ResultPlayerState->GetPawn());
    	int32 HPPercent =  (PlayerShip->GetCurrentHealth() /  PlayerShip->GetMaxHealth()) * 100;
    	BattleAnalytics.hp_win =  FMath::Max(HPPercent, 0);
    	int32 ShieldPercent =  (PlayerShip->GetCurrentShield() /  PlayerShip->GetMaxShield()) * 100;
    	BattleAnalytics.shield_win =  FMath::Max(ShieldPercent, 0);	
    	//Aimings
    	ResultPlayerState->UpdateAimingTime();
    	int32 aiming = ResultPlayerState->Analytics.aiming;
    	BattleAnalytics.aiming = aiming;
    			
    	TSharedPtr<FJsonObject> AnalyticsJsonData = FJsonObjectConverter::UStructToJsonObject<FBattleAnalytics>(BattleAnalytics);
    
    	// collect cards analytic
    			
    	// stop all card timers
    	Cast<USRCrewCardsActorComponent>(PlayerShip->GetCrewComponent())->StopAllCardsTimers();
    
    	TSharedPtr<FJsonObject> CardAnalyticsJsonObject = MakeShared<FJsonObject>();
    	for (const auto& CardAnalytic : BattleAnalytics.CharactersAnalytics)
    	{
    		TSharedPtr<FJsonObject> CardAnalyticsJsonData = FJsonObjectConverter::UStructToJsonObject<FCharacterBattleAnalytics>(CardAnalytic.Value);
    		CardAnalyticsJsonObject->SetObjectField(FString::FromInt(CardAnalytic.Key), CardAnalyticsJsonData);
    	}
    
    	AnalyticsJsonData->SetObjectField("characters_analytic", CardAnalyticsJsonObject);
    
    	return  AnalyticsJsonData;
    }*/
}

bool UPG_PlayerDataSubsystem::ParseCharacterItem(const TSharedPtr<FJsonObject>* JsonObject, UPG_Character_PlayerData* Character_Data)
{
	if(!JsonObject->IsValid())
		return false;
	
	JsonObject->Get()->TryGetStringField ("Nickname",Character_Data->CharacterName);
	
	/*const TArray<TSharedPtr<FJsonValue>>* ItemsJsonData;
	JsonObject->Get()->TryGetArrayField("Items",ItemsJsonData);*/
		
	/*TArray<FModifiableItemBase> ModifiableItem;
	for (const auto& PlayerValue : *ItemsJsonData)
	{
		const TSharedPtr<FJsonObject>* ItemValueObject;
		if(PlayerValue->TryGetObject(ItemValueObject))
		{
			FModifiableItemBase ItemData;
			if(FJsonObjectConverter::JsonObjectToUStruct<FModifiableItemBase>(ItemValueObject->ToSharedRef(), &ItemData, 0, 0))
				ModifiableItem.Add(ItemData);
		}
		/*const TSharedPtr<FJsonObject>* ItemValueObject;
		PlayerValue->TryGetObject(ItemValueObject);
		FModifiableItemBase ItemData;
		
		
		ItemValueObject->Get()->TryGetStringField("Id", ItemData.Id);		
		auto AttachmentsJsonData = ItemValueObject->Get()->GetArrayField("Attachments");
		for (const auto& AttachmentValue : AttachmentsJsonData)
		{
			const TSharedPtr<FJsonObject>* AttachmentsValueObject;  		
			AttachmentValue->TryGetObject(AttachmentsValueObject);
			FUnmodifiableItemBase AttachmentsItem;
			AttachmentsValueObject->Get()->TryGetStringField("Id", AttachmentsItem.Id);
			ItemData.Attachments.Add(AttachmentsItem);
		}*/
		
	//}
				
	TArray<FEquippedItemBase> EquipItems;
	EquipItems.Empty();
	const TSharedPtr<FJsonObject>* EquipValueObject;  		
	if(JsonObject->Get()->TryGetObjectField("Equip", EquipValueObject))
	{
		const TSharedPtr<FJsonObject>* WeaponOneValueObject;
		if(EquipValueObject->Get()->TryGetObjectField("WOne", WeaponOneValueObject))
		{
			FModifiableItemBase ItemData;
			if(FJsonObjectConverter::JsonObjectToUStruct<FModifiableItemBase>(WeaponOneValueObject->ToSharedRef(), &ItemData, 0, 0))
			{
				FEquippedItemBase Item;
				Item.Slot = "WOne";
				Item.ItemBase = ItemData;
				EquipItems.Add(Item);
			}
		}
		const TSharedPtr<FJsonObject>* HelmetValueObject;
		if(EquipValueObject->Get()->TryGetObjectField("Hel", HelmetValueObject))
		{
			FModifiableItemBase ItemData;
			if(FJsonObjectConverter::JsonObjectToUStruct<FModifiableItemBase>(HelmetValueObject->ToSharedRef(), &ItemData, 0, 0))
			{
				FEquippedItemBase Item;
				Item.Slot = "Hel";
				Item.ItemBase = ItemData;
				EquipItems.Add(Item);
			}
		}
		const TSharedPtr<FJsonObject>* ArmorValueObject;
		if(EquipValueObject->Get()->TryGetObjectField("Arm", ArmorValueObject))
		{
			FModifiableItemBase ItemData;
			if(FJsonObjectConverter::JsonObjectToUStruct<FModifiableItemBase>(ArmorValueObject->ToSharedRef(), &ItemData, 0, 0))
			{
				FEquippedItemBase Item;
				Item.Slot = "Arm";
				Item.ItemBase = ItemData;
				EquipItems.Add(Item);
			}
		}
	}
	Character_Data->EquipItems = EquipItems;
	//_EquipItems = EquipItems;
	return true;
}

/*TArray<FClothesInstance> UPG_ItemsSubsystem::CreateClothesPresets(UPG_ClothDataAsset* ClothData, FCIS_UpdateAnimClass& UpdateAnimClass)
{
	TArray<FClothesInstance> Clothes;
	Clothes.Empty();

	TArray<FClothesPresets>ClothesData = ClothData->GetClothesPresets();

	for(auto& Cloth : ClothesData)
	{
		FClothesInstance NewClothInst;
		NewClothInst.ClothType = Cloth.ClothType;
		NewClothInst.ClothItemDefinition = NewObject<UCIS_ItemDefinition>(this, Cloth.ClothDefinition);
		Clothes.Add(NewClothInst);
	}
	
	UpdateAnimClass = ClothData->GetUpdateAnimClass();
	
	return Clothes;
}*/
TArray<UCIS_ItemDefinition*> UPG_PlayerDataSubsystem::GetItemsDefinitions() const
{
	return ItemDefinitions;
}

UCIS_ItemDefinition* UPG_PlayerDataSubsystem::GetSlotDefinition(EPG_EWeaponSlots Slot) const
{
	UCIS_ItemDefinition* Item;
	Item = nullptr;
	switch (Slot)
	{
	case EPG_EWeaponSlots::PrimaryWeaponSlot:	
		 if(EquipItemDefinitions.Find("WOne"))		 	
			Item = *EquipItemDefinitions.Find("WOne");
		break;

		default:
			break;
	}
	return Item;
}

bool UPG_PlayerDataSubsystem::MakeLootDropList(FString LootDropID, TArray<FModifiableItemBase>& Items)
{
	const FString JsonFilePath = FPaths::ProjectSavedDir() + "JsonFiles/"+ LootDropID +".json";
	FString JsonString; //Json converted to FString
	
	FFileHelper::LoadFileToString(JsonString,*JsonFilePath);

	//Displaying the json in a string format inside the output log
	GLog->Log("Json String:");
	GLog->Log(JsonString);
	const TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonValue> JsonValue;	
	if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
	{
		TArray<FLootDropListItem> ModifiableItem;
		const TSharedPtr<FJsonObject>* JsonObject;
		JsonValue->TryGetObject(JsonObject);		

		const TArray<TSharedPtr<FJsonValue>>* ItemsJsonData;
		JsonObject->Get()->TryGetArrayField("Items",ItemsJsonData);

		for (const auto& PlayerValue : *ItemsJsonData)
		{
			const TSharedPtr<FJsonObject>* ItemValueObject;
			if(PlayerValue->TryGetObject(ItemValueObject))
			{
				FLootDropListItem ItemData;
				if(FJsonObjectConverter::JsonObjectToUStruct<FLootDropListItem>(ItemValueObject->ToSharedRef(), &ItemData, 0, 0))
					ModifiableItem.Add(ItemData);
			}
			
		}
	
		if(ModifiableItem.Num() == 0)
			return false;

		bool FixedDropChance = false;
		for(int32 i = 0; i < ModifiableItem.Num(); ++i)
		{
			if(ModifiableItem[i].DropChance == 100)
				FixedDropChance = true;
		}

		if(!FixedDropChance)
			ModifiableItem[0].DropChance = 100;
		TArray<FModifiableItemBase> LootDropItems;
		int32 DropIndex = UKismetMathLibrary::RandomIntegerInRange(1, 100);

		for(int32 i = 0; i < ModifiableItem.Num(); ++i)
		{
			if(ModifiableItem[i].DropChance >= DropIndex)
				LootDropItems.Add(ModifiableItem[i].GetItem());
		}
		Items = LootDropItems;
		return true;
	}
	return false;
}

void UPG_PlayerDataSubsystem::ReadLastCommanderFile()
{
	const FString JsonFilePath = FPaths::ProjectSavedDir() + "JsonFiles/LastCommander.json";
	FString JsonString; //Json converted to FString
	
	FFileHelper::LoadFileToString(JsonString,*JsonFilePath);

	//Displaying the json in a string format inside the output log
	GLog->Log("Json String:");
	GLog->Log(JsonString);
}

void UPG_PlayerDataSubsystem::ReadJsonFileAndMakeShipData(FString PlayerData, UPG_Ship_PlayerData* Ship_PlayerData)
{
	check(Ship_PlayerData);
	
	const TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(PlayerData);
	TSharedPtr<FJsonValue> JsonValue;
	
	if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
	{
		const TSharedPtr<FJsonObject>* JsonObject;
		JsonValue->TryGetObject(JsonObject);
		
		FString DateTo = "";
		JsonObject->Get()->TryGetStringField("DateTo", DateTo);
		FDateTime::Parse(DateTo, Ship_PlayerData->ShipData.DateTo);

		const TArray<TSharedPtr<FJsonValue>>* RoomsJsonData;
		JsonObject->Get()->TryGetArrayField("Rooms",RoomsJsonData);
		for (const auto& RoomValue : *RoomsJsonData)
		{
			const TSharedPtr<FJsonObject>* RoomValueObject;
			if(RoomValue->TryGetObject(RoomValueObject))
			{
				FShipRoomBase RoomData;
				if(FJsonObjectConverter::JsonObjectToUStruct<FShipRoomBase>(RoomValueObject->ToSharedRef(), &RoomData, 0, 0))
					Ship_PlayerData->ShipData.ShipRooms.Add(RoomData);
			}
		}
		const TArray<TSharedPtr<FJsonValue>>* SquadsJsonData;
		JsonObject->Get()->TryGetArrayField("Squads",SquadsJsonData);
		for (const auto& RoomValue : *RoomsJsonData)
		{
			const TSharedPtr<FJsonObject>* RoomValueObject;
			if(RoomValue->TryGetObject(RoomValueObject))
			{
				FShipRoomBase RoomData;
				if(FJsonObjectConverter::JsonObjectToUStruct<FShipRoomBase>(RoomValueObject->ToSharedRef(), &RoomData, 0, 0))
					Ship_PlayerData->ShipData.ShipRooms.Add(RoomData);
			}
		}
	}
}

void UPG_PlayerDataSubsystem::WriteJsonFileAboutShipData(APG_ShipBasePlayerController* PlayerController,
	APG_Hub_MapGenerator* MapGenerator)
{
	UPG_GameInstance* GI = GetGameGameInstance();
	if(!GI)
	check(PlayerController);
	
	const FString JsonFilePath = FPaths::ProjectSavedDir() + "JsonFiles/"+ GI->GetPlayerId() +"/ShipData.json";
	FString JsonString;

	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject);
	JsonObj->SetStringField("Player", GI->GetPlayerId());
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

	if(UPG_UtilityFunctionLibrary::GetUseBackEnd())
	{
		
	}
	else
	{
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);
	
		FFileHelper::SaveStringToFile(JsonString,*JsonFilePath);
	}
}

