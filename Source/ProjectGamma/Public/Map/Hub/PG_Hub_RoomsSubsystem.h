// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_Hub_MapGenerator.h"
#include "Core/PG_CoreTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PG_Hub_RoomsSubsystem.generated.h"

class APG_ShipBasePlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShipDataParsed,APG_ShipBasePlayerController*, PlayerController, FShipData, ShipData);
UCLASS()
class PROJECTGAMMA_API UPG_Hub_RoomsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION()
	void ReadJsonFileAndMakeShipData(APG_ShipBasePlayerController* PlayerController, FOnShipDataParsed ItemParsed);

	UFUNCTION()
	void WriteJsonFileAboutShipData(APG_ShipBasePlayerController* PlayerController, APG_Hub_MapGenerator* MapGenerator);

	
	/*template <typename T>
	bool GetItemIconPtr(const TFunction<FString()>& KeyGenerationFunctor, TSoftObjectPtr<UTexture2D>* IconPtr, UDataTable* DT)
	{
		auto DT = USRAssetManager::Get().GetItemsData()->ItemsIconsTable;
		if(DT)
		{
			FString Key ="";
			Key = KeyGenerationFunctor();
			const auto ModuleRow = DT->FindRow<T>(FName(*Key), "Context_String_Item_"+ Key);
			if(ModuleRow)
			{
				*IconPtr = ModuleRow->IconPtr;
				return true;
			}
		}
		return false;
	}*/
};
