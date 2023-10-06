// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PG_GameInstance.h"

#include "Core/PG_AssetManager.h"
#include "Core/PG_UtilityFunctionLibrary.h"
#include "InventorySystem/PG_ItemsSubsystem.h"
#include "Player/PG_PlayerController.h"

UPG_GameInstance::UPG_GameInstance()
{
	
}

void UPG_GameInstance::GoToMap(FString MapUrl)
{
	GetWorld()->ServerTravel(MapUrl);
}

void UPG_GameInstance::Init()
{
	Super::Init();

	TGetShipBaseData = [this](){ return GetShipBaseData();};
	TGetCommanderData = [this](){ return GetCommanderData();};
	TGetSquadData = [this](){ return GetSquadData();};
	PlayerDataFunction.Add(EPlayerDataType::ShipBase,TGetShipBaseData);
	PlayerDataFunction.Add(EPlayerDataType::Commander,TGetCommanderData);
	PlayerDataFunction.Add(EPlayerDataType::Squad,TGetSquadData);
}

FString UPG_GameInstance::GetShipBaseData()
{
	FString JsonString = "";
	return JsonString;
	if(UPG_UtilityFunctionLibrary::GetUseBackEnd())
	{
		
	}
	else
	{
		/*UPG_ItemsSubsystem* ItemsSubsystem = GetSubsystem<UPG_ItemsSubsystem>();
		check(ItemsSubsystem);
	
		FOnItemParsed ItemParsed;
		ItemParsed.AddDynamic(this, &ThisClass::SetParsedItemData);
		ItemsSubsystem->ReadJsonAndMakeItems(Player,ItemParsed);*/
	}
}

FString UPG_GameInstance::GetCommanderData()
{
	FString JsonString = "";
	const FString JsonFilePath = FPaths::ProjectSavedDir() + "JsonFiles/LastCommander.json";
	if(!FFileHelper::LoadFileToString(JsonString,*JsonFilePath))
	{
		auto DT = UPG_AssetManager::Get().GetItemsData()->DefaultItemPresets;
		if(DT)
		{		
			const auto ModuleRow = DT->FindRow<FDefaultItemPresets>(FName(TEXT("Default_Character")), "Context_String_Item_Default_Character");
			JsonString = ModuleRow->Preset;
		}		
	}
	//Displaying the json in a string format inside the output log
	GLog->Log("Json String:");
	GLog->Log(JsonString);

	return JsonString;
}

FString UPG_GameInstance::GetSquadData()
{
	FString JsonString = "";
	const FString JsonFilePath = FPaths::ProjectSavedDir() + "JsonFiles/" + GetPlayerId() +"/PlayerItem.json";
	if(!FFileHelper::LoadFileToString(JsonString,*JsonFilePath))
	{
		auto DT = UPG_AssetManager::Get().GetItemsData()->DefaultItemPresets;
		if(DT)
		{		
			const auto ModuleRow = DT->FindRow<FDefaultItemPresets>(FName(TEXT("Default_Character")), "Context_String_Item_Default_Character");
			JsonString = ModuleRow->Preset;
		}		
	}
	//Displaying the json in a string format inside the output log
	GLog->Log("Json String:");
	GLog->Log(JsonString);

	return JsonString;
}

void UPG_GameInstance::AuthorizePlayer(const FString& _PlayerId, const FString _Password)
{
	if(UPG_UtilityFunctionLibrary::GetUseBackEnd())
	{		
	}
	else
	{
		FString JsonString = "";
		const FString JsonFilePath = FPaths::ProjectSavedDir() + "JsonFiles/"+ PlayerId +"/ShipData.json";
		if(FFileHelper::LoadFileToString(JsonString,*JsonFilePath))
		{
			SetPlayerId(_PlayerId);
			AuthorizationComplete.Broadcast(false);
			AuthorizationComplete.Clear();
		}
	}
}

void UPG_GameInstance::SetPlayerId(const FString& _PlayerId)
{
	PlayerId = _PlayerId;
}

void UPG_GameInstance::GetPlayerData(UPG_QuickBarComponent* QuickBarComponent)
{
	FString PlayerData = "";
	PlayerData = PlayerDataFunction[QuickBarComponent->PlayerTypeData]();


	UPG_ItemsSubsystem* ItemsSubsystem = GetSubsystem<UPG_ItemsSubsystem>();
	check(ItemsSubsystem)
	FOnItemParsed ItemParsed;
	ItemParsed.AddDynamic(this, &ThisClass::SetParsedItemData);
	ItemsSubsystem->ParsePlayerData(QuickBarComponent,ItemParsed, PlayerData);
}

void UPG_GameInstance::SetParsedItemData(UPG_QuickBarComponent* QuickBarComponent,
	UPG_PlayerData* PlayerData)
{
	check(QuickBarComponent);

	//PlayerController->SetParsedItemData(PlayerItems, EquipItems);
}
