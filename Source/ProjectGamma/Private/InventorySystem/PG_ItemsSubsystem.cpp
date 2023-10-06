// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/PG_ItemsSubsystem.h"

#include "JsonObjectConverter.h"
#include "Core/CIS_CoreTypes.h"
#include "Core/PG_AssetManager.h"
#include "Core/PG_CoreTypes.h"
#include "Core/PG_GameInstance.h"
#include "InventorySystem/PG_QuickBarComponent.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_EquipmentItem.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_SetStats.h"
#include "InventorySystem/ItemsData/PG_Character_PlayerData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PG_PlayerController.h"


UPG_GameInstance* UPG_ItemsSubsystem::GetGameGameInstance()
{
	if(!GameInstance)
		GameInstance = Cast<UPG_GameInstance>(GetGameInstance());
	return GameInstance;
}

FString UPG_ItemsSubsystem::GetPlayerID()
{	
	return  GetGameGameInstance()->GetPlayerId();
}

void UPG_ItemsSubsystem::ParsePlayerData(UPG_QuickBarComponent* QuickBarComponent, FOnItemParsed ItemParsed, FString PlayerData)
{
	check(QuickBarComponent);
	
	const TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(PlayerData);
	TSharedPtr<FJsonValue> JsonValue;	
	if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
	{
		UPG_PlayerData* Character_Data = nullptr;
		const TSharedPtr<FJsonObject>* JsonObject;
		JsonValue->TryGetObject(JsonObject);
		
		switch (QuickBarComponent->PlayerTypeData)
		{
		case EPlayerDataType::Commander:
			Character_Data = NewObject<UPG_Character_PlayerData>(this);
			const TSharedPtr<FJsonObject>* CommanderObject;
			JsonObject->Get()->TryGetObjectField("Commander", CommanderObject);
			ParseCharacterItem(CommanderObject, Cast<UPG_Character_PlayerData>(Character_Data));
			QuickBarComponent->SetParsedPlayerData(Character_Data);
			break;
			
		case EPlayerDataType::Squad:
			break;
			
		case EPlayerDataType::ShipBase:
			break;
		}
				
		ItemParsed.Broadcast(QuickBarComponent, Character_Data);
		ItemParsed.Clear();
	}
	else
	{
		GLog->Log("couldn't deserialize");
	}
}

void UPG_ItemsSubsystem::SaveJsonFile()
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

bool UPG_ItemsSubsystem::ParseCharacterItem(const TSharedPtr<FJsonObject>* JsonObject, UPG_Character_PlayerData* Character_Data)
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
TArray<UCIS_ItemDefinition*> UPG_ItemsSubsystem::GetItemsDefinitions() const
{
	return ItemDefinitions;
}

UCIS_ItemDefinition* UPG_ItemsSubsystem::GetSlotDefinition(EPG_EWeaponSlots Slot) const
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

bool UPG_ItemsSubsystem::MakeLootDropList(FString LootDropID, TArray<FModifiableItemBase>& Items)
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

void UPG_ItemsSubsystem::ReadLastCommanderFile()
{
	const FString JsonFilePath = FPaths::ProjectSavedDir() + "JsonFiles/LastCommander.json";
	FString JsonString; //Json converted to FString
	
	FFileHelper::LoadFileToString(JsonString,*JsonFilePath);

	//Displaying the json in a string format inside the output log
	GLog->Log("Json String:");
	GLog->Log(JsonString);
}

