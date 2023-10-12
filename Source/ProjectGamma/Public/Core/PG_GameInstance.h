// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_CoreTypes.h"
#include "Engine/GameInstance.h"
#include "InventorySystem/PG_PlayerDataSubsystem.h"
#include "PG_GameInstance.generated.h"

/**
 * 
 */
class UPG_ProjectGamma_Settings;
class APG_PlayerController;
class UPG_QuickBarComponent;
class APG_GameMode;
UCLASS()
class PROJECTGAMMA_API UPG_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPG_GameInstance();

protected:

	UFUNCTION(BlueprintCallable)
	void GoToMap(FString MapUrl);

	//PLayer
	FString PlayerId = "Player_1";

	virtual void Init() override;

private:
		
	TMap<EPlayerDataType, TFunction<FString()>> PlayerDataFunction;

	TFunction<FString()> TGetShipBaseData;
	FString GetShipBaseData();
	TFunction<FString()> TGetCommanderData;
	FString GetCommanderData();
	TFunction<FString()> TGetSquadData;
	FString GetSquadData();
public:

	UFUNCTION(BlueprintCallable,  Category = "Authorization")
	void AuthorizePlayer(const FString& PlayerId, const FString Password);
	UPROPERTY(BlueprintAssignable)
	FOnAuthorizationComplete AuthorizationComplete;
	UFUNCTION(BlueprintCallable, Category = "Player Id")
	void SetPlayerId(const FString& PlayerId);
	
	UFUNCTION(BlueprintCallable, Category = "Player Id")
	FString GetPlayerId() const { return PlayerId; }

	void GetPlayerData(UPG_QuickBarComponent* QuickBarComponent, FOnItemParsed ItemParsed);

};

