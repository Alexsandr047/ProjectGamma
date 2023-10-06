// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_QuickBarComponent.h"
#include "Core/PG_CoreTypes.h"
#include "InventorySystem/Inventory/CIS_ItemDefinition.h"
#include "Map/Hub/PG_Hub_RoomsSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PG_ItemsSubsystem.generated.h"

DECLARE_DELEGATE_OneParam(FAsyncLoadClassObjectDelegate, const TArray<TSubclassOf<UObject>>&);

class APG_ShipBasePlayerController;
class UPG_GameInstance;
class UPG_Character_PlayerData;
class UPG_QuickBarComponent;
//DECLARE_MULTICAST_DELEGATE(FOnItemParsed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemParsed, class UPG_QuickBarComponent*, QuickBarComponent, UPG_PlayerData*, PlayerData);

UCLASS()
class PROJECTGAMMA_API UPG_ItemsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPG_GameInstance* GetGameGameInstance();
	FString GetPlayerID();
	UFUNCTION()
	void ParsePlayerData(UPG_QuickBarComponent* QuickBarComponent, FOnItemParsed ItemParsed, FString PlayerData);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SaveJsonFile();

	bool ParseCharacterItem(const TSharedPtr<FJsonObject>* JsonObject, UPG_Character_PlayerData* Character_Data);
	UFUNCTION(BlueprintCallable)
	TArray<UCIS_ItemDefinition*> GetItemsDefinitions() const;

	UFUNCTION(BlueprintCallable)
	UCIS_ItemDefinition* GetSlotDefinition(EPG_EWeaponSlots Slot) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool MakeLootDropList(FString LootDropID, TArray<FModifiableItemBase>& Items);

	UFUNCTION(BlueprintCallable)
	void ReadLastCommanderFile();
protected:
	
	UPROPERTY()
	TMap<FString, UCIS_ItemDefinition*> EquipItemDefinitions;
	UPROPERTY()
	TArray<UCIS_ItemDefinition*> ItemDefinitions;

	UPROPERTY()
	UPG_GameInstance* GameInstance = nullptr;

};
 