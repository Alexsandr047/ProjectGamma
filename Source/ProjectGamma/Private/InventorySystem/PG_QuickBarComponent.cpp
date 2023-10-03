// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/PG_QuickBarComponent.h"

#include "Core/CIS_UtilityFunctionLibrary.h"
#include "Core/PG_AssetManager.h"
#include "Core/ItemsData/PG_Character_PlayerData.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "InventorySystem/Inventory/CIS_ItemInstance.h"
#include "InventorySystem/Inventory/CIS_ItemManagerComponent.h"
#include "InventorySystem/Equipment/CIS_EquipmentInstance.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_EquipmentItem.h"
#include "InventorySystem/Inventory/Fragments/CIS_ItemFragment_SetStats.h"
#include "Net/UnrealNetwork.h"
#include "Player/PG_PlayerController.h"
#include "Weapons/PG_WeaponInstance.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_PG_Inventory_Message_ItemsInInvetory, "CIS.Inventory.Message.Items");


UPG_QuickBarComponent::UPG_QuickBarComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UPG_QuickBarComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UPG_QuickBarComponent::OnRep_SlotChanged()
{
	UpdateEquipSlot.Broadcast();
}

void UPG_QuickBarComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, HelmetSlot);
	DOREPLIFETIME(ThisClass, ArmorSlot);
	DOREPLIFETIME(ThisClass, PrimaryWeaponSlot);
	DOREPLIFETIME(ThisClass, SpecialWeaponSlot);
	DOREPLIFETIME(ThisClass, PistolWeaponSlot);
	DOREPLIFETIME(ThisClass, PrimaryWeaponItem);
	DOREPLIFETIME(ThisClass, SpecialWeaponItem);
	DOREPLIFETIME(ThisClass, PistolWeaponItem);
}
APG_PlayerController* UPG_QuickBarComponent::GetPlayerController() const
{
	return Cast<APG_PlayerController>(GetOwner());
}
void UPG_QuickBarComponent::PreparingItemDefinitions(TArray<FModifiableItemBase> ParsedItems, FOnPreparingItemDefinitions OnPreparingItemDefinitions)
{
	TArray<UCIS_ItemDefinition*> ItemDefinitions;
	ItemDefinitions.Empty();

	if(ParsedItems.Num() > 0)
	{
		FAsyncLoadClassObjectDelegate* OnAsyncLoadOptionsDefenitionsCompleteDelagate = new FAsyncLoadClassObjectDelegate();
		TArray<TSoftClassPtr<UObject>> DefinitionsToLoad;

		TMap<FString, TSubclassOf<UObject>> ItemsSoftDef;
				ItemsSoftDef.Reset();
		for(auto& ItemDef: ParsedItems)
		{
			if(!ItemsSoftDef.Find(ItemDef.Id))
				ItemsSoftDef.Add(ItemDef.Id, nullptr);
			for(auto& Attach : ItemDef.Attachments)
			{
				if(!ItemsSoftDef.Find(Attach.Id))
					ItemsSoftDef.Add(Attach.Id, nullptr);
			}
		}
		TArray<FString> Keys;
		ItemsSoftDef.GetKeys(Keys);
		for (auto& Key : Keys)
		{
			
			auto DT = UPG_AssetManager::Get().GetItemsData()->ItemsDataTable;
			if(DT)
			{
				const auto ModuleRow = DT->FindRow<FItemDefinitionData>(FName(*Key), "Context_String_Item_"+ Key);
			
				if(ItemsSoftDef.Remove(Key))
					DefinitionsToLoad.Add(ModuleRow->ItemDefinition);
			}		
		}
		OnAsyncLoadOptionsDefenitionsCompleteDelagate->BindLambda([this, Keys, ItemsSoftDef, ParsedItems,OnPreparingItemDefinitions](const TArray<TSubclassOf<UObject>>& Definitions)
		{
			if (Definitions[0])
			{
				TMap<FString, TSubclassOf<UObject>> LambdaItemsSoftDef;
				TArray<FString> LambdaKeys;
				LambdaItemsSoftDef = ItemsSoftDef;
				LambdaKeys = Keys;
				APG_PlayerController* PC = GetPlayerController();
				if(!PC)
					return;			
				bool bCanInit = false;
				for(int32 i = 0 ; i < Definitions.Num(); ++i)
				{
					if(LambdaKeys.Num() == Definitions.Num())
					{
						LambdaItemsSoftDef.Add(LambdaKeys[i],Definitions[i]);
						bCanInit = true;
					}				
				}
				if(!bCanInit)
					return;
			
				TArray<UCIS_ItemDefinition*> ItemDefinitions;
				ItemDefinitions.Empty();
				for(auto& Item: ParsedItems)
				{
					if(LambdaItemsSoftDef.Find(Item.Id))
					{
						if(auto ItemDef = NewObject<UCIS_ItemDefinition>(this, *LambdaItemsSoftDef.Find(Item.Id)))
						{
							TArray<UCIS_ItemDefinition*> AttachDefinitions;
							AttachDefinitions.Empty();
									
							for(auto& Attach : Item.Attachments)
							{
								if(LambdaItemsSoftDef.Find(Attach.Id))
								{
									if(auto AttachDef = NewObject<UCIS_ItemDefinition>(this, *LambdaItemsSoftDef.Find(Attach.Id)))
									{
										AttachDefinitions.Add(AttachDef);
									}
								}
							}
							if(UCIS_ItemFragment_EquipmentItem* EquipmentFragment =  Cast<UCIS_ItemFragment_EquipmentItem>(ItemDef->FindFragmentByClass(UCIS_ItemFragment_EquipmentItem::StaticClass())))
								EquipmentFragment->SetAttachmentDefinition(AttachDefinitions);

							if(UCIS_ItemFragment_SetStats* SetFragment =  Cast<UCIS_ItemFragment_SetStats>(ItemDef->FindFragmentByClass(UCIS_ItemFragment_SetStats::StaticClass())))
							{
								for(auto& Tag: Item.TagStacks)
								{
									auto DT = UPG_AssetManager::Get().GetItemsData()->ItemsTagsDataTable;
									if(DT)
									{
										const auto ItemTag = DT->FindRow<FItemTags>(FName(*Tag.Id), "Context_String_Item_Tag"+ Tag.Id);
										if(ItemTag->ItemTag.IsValid())
											SetFragment->SetTagStats(ItemTag->ItemTag, Tag.Stack);
									}
								}
							}						
							ItemDefinitions.Add(ItemDef);
						}
					}
				}
				OnPreparingItemDefinitions.Execute(ItemDefinitions);
			}			
		});
		UCIS_UtilityFunctionLibrary::AsyncLoadClassObject(GetWorld(),DefinitionsToLoad, OnAsyncLoadOptionsDefenitionsCompleteDelagate);
	}
}
void UPG_QuickBarComponent::InitEquip(UCIS_ItemManagerComponent* _ItemManager)
{
	if(!_ItemManager)
		return;
	ItemManager = _ItemManager;

	if(APG_PlayerController* PC = GetPlayerController())
	{
		if(UPG_Character_PlayerData* CharacterData = Cast<UPG_Character_PlayerData>( PC->GetPlayerData()))
		{
			TArray<FString> EquipSlots;
			TArray<FModifiableItemBase> ParsedItems;
			for(auto& EquipDef: CharacterData->EquipItems)
			{
				EquipSlots.Add(EquipDef.Slot);
				ParsedItems.Add(EquipDef.ItemBase);
			}
			
			FOnPreparingItemDefinitions* OnPreparingItemDefinitions = new FOnPreparingItemDefinitions();
			OnPreparingItemDefinitions->BindLambda([this, EquipSlots](const TArray<UCIS_ItemDefinition*>& _ItemDefinitions)
			{
				TMap<FString, UCIS_ItemDefinition*> EquipItemDefinitions;
				for(int32 i =0; i < EquipSlots.Num(); ++i )
				{
					EquipItemDefinitions.Add(EquipSlots[i],_ItemDefinitions[i]);
				}
				if(EquipItemDefinitions.Find("WOne"))
					EquipWeaponInWeaponSlot(*EquipItemDefinitions.Find("WOne"), EPG_EWeaponSlots::PrimaryWeaponSlot);
				if(EquipItemDefinitions.Find("Arm"))
					EquipEquipmentTypeOnCharacter(*EquipItemDefinitions.Find("Arm"), EPG_EquipmentType::Armor);

				ItemLoaded.Broadcast();
				ItemLoaded.Clear();
			});
			PreparingItemDefinitions(ParsedItems, *OnPreparingItemDefinitions);
		}
	}
}

/*void UPG_QuickBarComponent::InitEquipAndInventory(UCIS_ItemManagerComponent* _ItemManager)
{
	if(!_ItemManager)
		return;
	ItemManager = _ItemManager;
	
	if(APG_PlayerController* PC = GetPlayerController())
	{
		FAsyncLoadClassObjectDelegate* OnAsyncLoadOptionsDefenitionsCompleteDelagate = new FAsyncLoadClassObjectDelegate();
		TArray<TSoftClassPtr<UObject>> DefinitionsToLoad;

		TMap<FString, TSubclassOf<UObject>> ItemsSoftDef;
		TArray<FString> Keys;
		ItemsSoftDef.Reset();
		for(auto& ItemDef: PC->GetPlayerItems())
		{
			if(!ItemsSoftDef.Find(ItemDef.Id))
				ItemsSoftDef.Add(ItemDef.Id, nullptr);
			for(auto& Attach : ItemDef.Attachments)
			{
				if(!ItemsSoftDef.Find(Attach.Id))
					ItemsSoftDef.Add(Attach.Id, nullptr);
			}
		}
		TMap<FString, FModifiableItemBase> EquipItems;
		for(auto& EquipDef: PC->GetEquipItems())
		{
			EquipItems.Add(EquipDef.Slot, EquipDef.ItemBase);
		}
		TArray<FString> EquipKeys;
		EquipItems.GetKeys(EquipKeys);
		for(auto& Key : EquipKeys)
		{
			if(!ItemsSoftDef.Find(EquipItems.Find(Key)->Id))
				ItemsSoftDef.Add(EquipItems.Find(Key)->Id, nullptr);
			for(auto& Attach : EquipItems.Find(Key)->Attachments)
			{
				if(!ItemsSoftDef.Find(Attach.Id))
					ItemsSoftDef.Add(Attach.Id, nullptr);
			}
		}	
		Keys.Empty();	
		ItemsSoftDef.GetKeys(Keys);
		for (auto& Key : Keys)
		{
			
			auto DT = UPG_AssetManager::Get().GetItemsData()->ItemsDataTable;
			if(DT)
			{
				const auto ModuleRow = DT->FindRow<FItemDefinitionData>(FName(*Key), "Context_String_Item_"+ Key);
			
				if(ItemsSoftDef.Remove(Key))
					DefinitionsToLoad.Add(ModuleRow->ItemDefinition);
			}		
		}
		
		OnAsyncLoadOptionsDefenitionsCompleteDelagate->BindLambda([this, Keys, ItemsSoftDef](const TArray<TSubclassOf<UObject>>& Definitions)
		{
			if (Definitions[0])
			{
				TMap<FString, TSubclassOf<UObject>> LambdaItemsSoftDef;
				TArray<FString> LambdaKeys;
				LambdaItemsSoftDef = ItemsSoftDef;
				LambdaKeys = Keys;
				APG_PlayerController* PC = GetPlayerController();
				if(!PC)
					return;			
				bool bCanInit = false;
				for(int32 i = 0 ; i < Definitions.Num(); ++i)
				{
					if(LambdaKeys.Num() == Definitions.Num())
					{
						LambdaItemsSoftDef.Add(LambdaKeys[i],Definitions[i]);
						bCanInit = true;
					}				
				}
				if(!bCanInit)
					return;
			
				TArray<UCIS_ItemDefinition*> ItemDefinitions;
				ItemDefinitions.Empty();
				for(auto& Item: PC->GetPlayerItems())
				{
					if(LambdaItemsSoftDef.Find(Item.Id))
					{
						if(auto ItemDef = NewObject<UCIS_ItemDefinition>(this, *LambdaItemsSoftDef.Find(Item.Id)))
						{
							TArray<UCIS_ItemDefinition*> AttachDefinitions;
							AttachDefinitions.Empty();
					
							for(auto& Attach : Item.Attachments)
							{
								if(LambdaItemsSoftDef.Find(Attach.Id))
								{
									if(auto AttachDef = NewObject<UCIS_ItemDefinition>(this, *LambdaItemsSoftDef.Find(Attach.Id)))
									{
										AttachDefinitions.Add(AttachDef);
									}
								}
							}
							if(UCIS_ItemFragment_EquipmentItem* EquipmentFragment =  Cast<UCIS_ItemFragment_EquipmentItem>(ItemDef->FindFragmentByClass(UCIS_ItemFragment_EquipmentItem::StaticClass())))
								EquipmentFragment->SetAttachmentDefinition(AttachDefinitions);

							if(UCIS_ItemFragment_SetStats* SetFragment =  Cast<UCIS_ItemFragment_SetStats>(ItemDef->FindFragmentByClass(UCIS_ItemFragment_SetStats::StaticClass())))
							{
								for(auto& Tag: Item.TagStacks)
								{
									auto DT = UPG_AssetManager::Get().GetItemsData()->ItemsTagsDataTable;
									if(DT)
									{
										const auto ItemTag = DT->FindRow<FItemTags>(FName(*Tag.Id), "Context_String_Item_Tag"+ Tag.Id);
										if(ItemTag->ItemTag.IsValid())
											SetFragment->SetTagStats(ItemTag->ItemTag, Tag.Stack);
									}
								}
							}						
							ItemDefinitions.Add(ItemDef);
						}
					}
				}
				TMap<FString, FModifiableItemBase> EquipItems;
				for(auto& EquipDef: PC->GetEquipItems())
				{
					EquipItems.Add(EquipDef.Slot, EquipDef.ItemBase);
				}
				TArray<FString> EquipSlots;
				EquipItems.GetKeys(EquipSlots);
				
				TMap<FString, UCIS_ItemDefinition*> EquipItemDefinitions;
				EquipItemDefinitions.Reset();
				for(auto& Slot : EquipSlots)
				{
					FModifiableItemBase ItemData = *EquipItems.Find(Slot);
					if(LambdaItemsSoftDef.Find(*ItemData.Id))
					{
						if(auto ItemDef = NewObject<UCIS_ItemDefinition>(this, *LambdaItemsSoftDef.Find(*ItemData.Id)))
						{
							TArray<UCIS_ItemDefinition*> AttachDefinitions;
							AttachDefinitions.Empty();
							for(auto& Attach : ItemData.Attachments)
							{
								if(LambdaItemsSoftDef.Find(Attach.Id))
								{
									if(auto AttachDef = NewObject<UCIS_ItemDefinition>(this, *LambdaItemsSoftDef.Find(Attach.Id)))
									{
										AttachDefinitions.Add(AttachDef);
									}
								}
							}
							if(UCIS_ItemFragment_EquipmentItem* EquipmentFragment =  Cast<UCIS_ItemFragment_EquipmentItem>(ItemDef->FindFragmentByClass(UCIS_ItemFragment_EquipmentItem::StaticClass())))
								EquipmentFragment->SetAttachmentDefinition(AttachDefinitions);
							if(UCIS_ItemFragment_SetStats* SetFragment =  Cast<UCIS_ItemFragment_SetStats>(ItemDef->FindFragmentByClass(UCIS_ItemFragment_SetStats::StaticClass())))
							{
								for(auto& Tag: ItemData.TagStacks)
								{
									auto DT = UPG_AssetManager::Get().GetItemsData()->ItemsDataTable;
									if(DT)
									{
										const auto ItemTag = DT->FindRow<FItemTags>(FName(*Tag.Id), "Context_String_Item_Tag"+ Tag.Id);
										if(ItemTag->ItemTag.IsValid())
											SetFragment->SetTagStats(ItemTag->ItemTag, Tag.Stack);
									}
								}
							}						
							EquipItemDefinitions.Add(Slot, ItemDef);
						}
					}				
				}
				LoadItemsToInventory(ItemManager,ItemDefinitions);
				if(EquipItemDefinitions.Find("WOne"))
					EquipWeaponInWeaponSlot(*EquipItemDefinitions.Find("WOne"), EPG_EWeaponSlots::PrimaryWeaponSlot);
				if(EquipItemDefinitions.Find("Arm"))
					EquipEquipmentTypeOnCharacter(*EquipItemDefinitions.Find("Arm"), EPG_EquipmentType::Armor);
			}
		});
		UCIS_UtilityFunctionLibrary::AsyncLoadClassObject(GetWorld(),DefinitionsToLoad, OnAsyncLoadOptionsDefenitionsCompleteDelagate);
	}	
}*/



void UPG_QuickBarComponent::InitItemsToInventory(TArray<FModifiableItemBase> Items)
{
	FAsyncLoadClassObjectDelegate* OnAsyncLoadOptionsDefenitionsCompleteDelagate = new FAsyncLoadClassObjectDelegate();
	TArray<TSoftClassPtr<UObject>> DefinitionsToLoad;
	
	TMap<FString, TSubclassOf<UObject>> ItemsSoftDef;
	TArray<FString> Keys;
	for(auto& ItemDef: Items)
	{
		if(!ItemsSoftDef.Find(ItemDef.Id))
			ItemsSoftDef.Add(ItemDef.Id, nullptr);
		for(auto& Attach : ItemDef.Attachments)
		{
			if(!ItemsSoftDef.Find(Attach.Id))
				ItemsSoftDef.Add(Attach.Id, nullptr);
		}
	}
	ItemsSoftDef.GetKeys(Keys);
	for (auto& Key : Keys)
	{
			
		auto DT = UPG_AssetManager::Get().GetItemsData()->ItemsDataTable;
		if(DT)
		{
			const auto ModuleRow = DT->FindRow<FItemDefinitionData>(FName(*Key), "Context_String_Item_"+ Key);
			
			if(ItemsSoftDef.Remove(Key))
				DefinitionsToLoad.Add(ModuleRow->ItemDefinition);
		}		
	}

	/*OnAsyncLoadOptionsDefenitionsCompleteDelagate->BindLambda([this, ItemsSoftDef, Keys](const TArray<TSubclassOf<UObject>>& Definitions)
	{
		if (Definitions[0])
		{
			TMap<FString, TSubclassOf<UObject>> LambdaItemsSoftDef;
			TArray<FString> LambdaKeys;
			LambdaItemsSoftDef = ItemsSoftDef;
			LambdaKeys = Keys;
			APG_PlayerController* PC = GetPlayerController();
			if(!PC)
				return;			
			bool bCanInit = false;
			for(int32 i = 0 ; i < Definitions.Num(); ++i)
			{
				if(LambdaKeys.Num() == Definitions.Num())
				{
					LambdaItemsSoftDef.Add(LambdaKeys[i],Definitions[i]);
					bCanInit = true;
				}				
			}
			if(!bCanInit)
				return;
			
			TArray<UCIS_ItemDefinition*> ItemDefinitions;
			ItemDefinitions.Empty();
			for(auto& Item: PC->GetPlayerItems())
			{
				if(LambdaItemsSoftDef.Find(Item.Id))
				{
					if(auto ItemDef = NewObject<UCIS_ItemDefinition>(this, *LambdaItemsSoftDef.Find(Item.Id)))
					{
						TArray<UCIS_ItemDefinition*> AttachDefinitions;
						AttachDefinitions.Empty();
					
						for(auto& Attach : Item.Attachments)
						{
							if(LambdaItemsSoftDef.Find(Attach.Id))
							{
								if(auto AttachDef = NewObject<UCIS_ItemDefinition>(this, *LambdaItemsSoftDef.Find(Attach.Id)))
								{
									AttachDefinitions.Add(AttachDef);
								}
							}
						}
						if(UCIS_ItemFragment_EquipmentItem* EquipmentFragment =  Cast<UCIS_ItemFragment_EquipmentItem>(ItemDef->FindFragmentByClass(UCIS_ItemFragment_EquipmentItem::StaticClass())))
							EquipmentFragment->SetAttachmentDefinition(AttachDefinitions);

						if(UCIS_ItemFragment_SetStats* SetFragment =  Cast<UCIS_ItemFragment_SetStats>(ItemDef->FindFragmentByClass(UCIS_ItemFragment_SetStats::StaticClass())))
						{
							for(auto& Tag: Item.TagStacks)
							{
								auto DT = UPG_AssetManager::Get().GetItemsData()->ItemsTagsDataTable;
								if(DT)
								{
									const auto ItemTag = DT->FindRow<FItemTags>(FName(*Tag.Id), "Context_String_Item_Tag"+ Tag.Id);
									if(ItemTag->ItemTag.IsValid())
										SetFragment->SetTagStats(ItemTag->ItemTag, Tag.Stack);
								}
							}
						}						
						ItemDefinitions.Add(ItemDef);
					}
				}
			}
			LoadItemsToInventory(ItemManager,ItemDefinitions);			
		}
	});
	UCIS_UtilityFunctionLibrary::AsyncLoadClassObject(GetWorld(),DefinitionsToLoad, OnAsyncLoadOptionsDefenitionsCompleteDelagate);*/
}

void UPG_QuickBarComponent::EquipEquipmentTypeOnCharacter(UCIS_ItemDefinition* Definition, EPG_EquipmentType EquipmentType)
{
	UCIS_ItemManagerComponent* InventoryManager = FindItemManager();
	{
		UCIS_ItemInstance* Item = InventoryManager->AddItemDefinition(Definition);
		switch (EquipmentType)
		{
		case EPG_EquipmentType::Helmet:
			EquipItemInSlot(Item,HelmetItem,HelmetSlot,"root");
			break;

		case  EPG_EquipmentType::Armor:
			EquipItemInSlot(Item,ArmorItem,ArmorSlot,"root");
			break;

		default:
			break;
		}
	}
}

void UPG_QuickBarComponent::EquipWeaponInWeaponSlot(UCIS_ItemDefinition* Definition, EPG_EWeaponSlots WeaponSlots)
{
	UCIS_ItemManagerComponent* InventoryManager = FindItemManager();
	{
		
		UCIS_ItemInstance* Item = InventoryManager->AddItemDefinition(Definition);
		switch (WeaponSlots)
		{
		case EPG_EWeaponSlots::PrimaryWeaponSlot:
			EquipItemInSlot(Item,PrimaryWeaponItem,PrimaryWeaponSlot, "weapon_slot_00");
			break;

		default:
			break;
		}
	}
}

void UPG_QuickBarComponent::UseItemOnEquipmentSlot(EPG_EWeaponSlots EquipmentsSlots, int32 InventorySlot)
{
	if(InventorySlot == -1 || InventorySlot >= GetInventorySlots().Num())
		return;
	
	UCIS_ItemInstance* Item = GetInventorySlots()[InventorySlot];
	if(GetInventorySlots()[InventorySlot])
		ClearInventorySlot(InventorySlot);
	UCIS_ItemInstance* EQuipSlot;
	
	switch (EquipmentsSlots)
	{
	case EPG_EWeaponSlots::PrimaryWeaponSlot:
			EQuipSlot = PrimaryWeaponSlot;			
			ClearItemInSlot(PrimaryWeaponSlot, PrimaryWeaponItem);
			if(EQuipSlot)
				AddItemInInventorySlot(InventorySlot, EQuipSlot);
			if(Item)
			{
				EquipItemInSlot(Item,PrimaryWeaponItem,PrimaryWeaponSlot,"hand_r");
				OnRep_SlotChanged();
			}
		
		break;

	default:
		break;
	}
}

void UPG_QuickBarComponent::GetWeaponItem(EPG_EWeaponSlots EquipmentsSlots, UCIS_EquipmentInstance*& Weapon)
{
	switch (EquipmentsSlots)
	{
	case EPG_EWeaponSlots::PrimaryWeaponSlot:
	Weapon = PrimaryWeaponItem;
		break;
	case EPG_EWeaponSlots::SpecialWeaponSlot:
		Weapon = SpecialWeaponItem;
		break;
	case EPG_EWeaponSlots::PistolWeaponSlots:
		Weapon = PistolWeaponItem;
		break;
	default:
		break;
	}
}

void UPG_QuickBarComponent::GetCurrentlyInUseWeapon(UCIS_EquipmentInstance*& Weapon)
{
	UCIS_EquipmentInstance* CurrentWeapon;
	CurrentWeapon = nullptr;

	if(PrimaryWeaponItem && Cast<UPG_WeaponInstance>(PrimaryWeaponItem)->GetIsCurrentWeapon())
		CurrentWeapon = PrimaryWeaponItem;
	if(SpecialWeaponItem && Cast<UPG_WeaponInstance>(SpecialWeaponItem)->GetIsCurrentWeapon())
		CurrentWeapon = SpecialWeaponItem;
	if(PistolWeaponItem && Cast<UPG_WeaponInstance>(PistolWeaponItem)->GetIsCurrentWeapon())
		CurrentWeapon = PistolWeaponItem;
	Weapon = CurrentWeapon;
}

void UPG_QuickBarComponent::GetWeaponSlots(EPG_EWeaponSlots EquipmentsSlots, UCIS_ItemInstance*& WeaponSlot)
{
	switch (EquipmentsSlots)
	{
	case EPG_EWeaponSlots::PrimaryWeaponSlot:
		WeaponSlot = PrimaryWeaponSlot;
		break;
	case EPG_EWeaponSlots::SpecialWeaponSlot:
		WeaponSlot = SpecialWeaponSlot;
		break;
	case EPG_EWeaponSlots::PistolWeaponSlots:
		WeaponSlot = PistolWeaponSlot;
		break;
	default:
		break;
	}
}

void UPG_QuickBarComponent::GetItemsInInventory()
{
	FPG_GetItemsInInventoryMessage Message;
	Message.Items = GetInventorySlots();

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetOwner()->GetWorld());
	MessageSystem.BroadcastMessage(TAG_PG_Inventory_Message_ItemsInInvetory, Message);
}

void  UPG_QuickBarComponent::RemoveAllItemsFromSlots()
{
	if(GetInventorySlots().Num() > 0)
	{
		for(int32 i = 0; i < GetInventorySlots().Num(); ++i)
		{
			RemoveItemInInventorySlot(i);
		}
	}
	//Clear Equipment
	ClearItemInSlot(HelmetSlot,HelmetItem);
	ClearItemInSlot(ArmorSlot, ArmorItem);
	//Clear Weapons
	ClearItemInSlot(PrimaryWeaponSlot, PrimaryWeaponItem);
	ClearItemInSlot(SpecialWeaponSlot, SpecialWeaponItem);
	ClearItemInSlot(PistolWeaponSlot, PistolWeaponItem);
	/*ClearItemInSlot();
	ClearItemInSlot();
	ClearItemInSlot();
	ClearItemInSlot();*/
}

bool UPG_QuickBarComponent::AddItemInEmptySlotInventory(UCIS_ItemInstance* Item)
{
	int32 SlotIndex = -1;

	for(int32 i = 0; i < GetInventorySlots().Num(); ++ i)
	{
		if(!GetInventorySlots()[i])
		{
			SlotIndex = i;
			break;
		}
	}
	if(SlotIndex == -1 || !Item)
		return false;

	
	return AddItemInInventorySlot(SlotIndex, Item);
}


