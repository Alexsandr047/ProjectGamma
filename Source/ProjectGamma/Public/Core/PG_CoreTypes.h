// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/CIS_CoreTypes.h"
#include "Engine/DataTable.h"
#include "PG_CoreTypes.generated.h"
USTRUCT(BlueprintType)
struct FDefaultItemPresets:  public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Preset = "";
};
USTRUCT(BlueprintType)
struct FDescriptionItem :  public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> IconItem = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description;
};

USTRUCT(BlueprintType)
struct FItemTagStacks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Id ="";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stack = 0;
};

USTRUCT(BlueprintType)
struct FUnmodifiableItemBase :  public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Id ="";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemTagStacks> TagStacks;
};

USTRUCT(BlueprintType)
struct FModifiableItemBase :  public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Id = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemTagStacks> TagStacks;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FUnmodifiableItemBase> Attachments;
	
};
USTRUCT(BlueprintType)
struct FEquippedItemBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Slot = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FModifiableItemBase ItemBase;
};
USTRUCT(BlueprintType)
struct FLootDropListItem :  public FModifiableItemBase
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DropChance = 100;

	FModifiableItemBase GetItem()
	{
		FModifiableItemBase Item;
		Item.Id = Id;
		Item.TagStacks = TagStacks;
		Item.Attachments = Attachments;
		return Item;
	}
};

USTRUCT(BlueprintType)
struct FItemDefinitionData :  public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UCIS_ItemDefinition> ItemDefinition;
	
};

USTRUCT(BlueprintType)
struct FItemTags :  public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag;
	
};
USTRUCT(BlueprintType)
struct FLootBoxAndItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* LootBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FModifiableItemBase> Items;
};

UENUM(BlueprintType)
enum class ERoomCondition  : uint8
{
	Debris,
	Clearing,
	Empty,
	Building,
	Built
};
UENUM(BlueprintType)
enum class ERoomType  : uint8
{
	Empty,
	Debris,
	Laboratory,
	Workshop,
	PowerRelay
};

USTRUCT(BlueprintType)
struct FSkillParseData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillId;

	UPROPERTY()
	bool Unlock;
	
	FSkillParseData()
	{
		SkillId = "";
		Unlock = false;
	}
	FSkillParseData(FString _SkillId, bool _Unlock)
	{
		SkillId = _SkillId;
		Unlock = _Unlock;
	}
	FSkillParseData& operator =( const FSkillParseData& other)
	{
		this->SkillId = other.SkillId;
		this->Unlock = other.Unlock;
		return *this;
	}

	bool IsValid() const
	{
		if(SkillId != "")
			return true;
		return false;
	}
};

USTRUCT(BlueprintType)
struct FShipRoomBase :  public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Id = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FinishDay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomCondition RoomCondition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomType RoomType;
};

USTRUCT(BlueprintType)
struct FShipResourceBase :  public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Id = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 0;

	FShipResourceBase(){};
	FShipResourceBase(FString _Id,  int32 _Quantity)
	{
		Id = _Id;
		Quantity = _Quantity;
	}
};

USTRUCT(BlueprintType)
struct FSquad
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Id = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BannerID = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CommanderId = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FighterOne = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FighterTwo = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FighterThree = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CurrentSquad = false;
	
};

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapUrl = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapDescription = "";
};

UENUM(BlueprintType)
enum class EPlayerDataType  : uint8
{
	ShipBase,
	Squad,
	Commander
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthorizationComplete, bool, bStartTutorial);


UCLASS()
class PROJECTGAMMA_API UPG_CoreTypes : public UCIS_CoreTypes
{
	GENERATED_BODY()

public:

	
};
