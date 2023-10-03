// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CIS_UtilityFunctionLibrary.h"

#include "Core/CIS_AssetManager.h"
#include "Engine/StreamableManager.h"


void UCIS_UtilityFunctionLibrary::AsyncLoadObject(UObject* WorldContextObject, const TArray<TSoftObjectPtr<>>& Assets,
                                                  FAsyncLoadObjectDelegate* OnAsyncLoadObjectCompleteDelagate)
{
	FStreamableManager& AssetLoader = UAssetManager::Get().GetStreamableManager();

	TArray<FSoftObjectPath> ReferenceList;

	for (auto& Asset : Assets)
	{
		ReferenceList.Add(Asset.ToSoftObjectPath());	
	}
	AssetLoader.RequestAsyncLoad(ReferenceList, FStreamableDelegate::CreateStatic(&UCIS_UtilityFunctionLibrary::AsyncLoadObjectComplete, WorldContextObject, Assets, ReferenceList, OnAsyncLoadObjectCompleteDelagate));
}

void UCIS_UtilityFunctionLibrary::AsyncLoadObjectComplete(UObject* WorldContextObject, TArray<TSoftObjectPtr<>> Assets,
	TArray<FSoftObjectPath> References, FAsyncLoadObjectDelegate* Callback)
{
	TArray<UObject*> AssetList;
	
	for (auto& AssetPtr : Assets)
	{
		UObject* Asset = AssetPtr.Get();
		if (!Asset) return;
		AssetList.Add(Asset);
	}
	Callback->ExecuteIfBound(AssetList);
}

void UCIS_UtilityFunctionLibrary::AsyncLoadClassObject(UObject* WorldContextObject, const TArray<TSoftClassPtr<>>& Assets,
	FAsyncLoadClassObjectDelegate* OnAsyncLoadObjectCompleteDelagate)
{
	FStreamableManager& AssetLoader = UAssetManager::Get().GetStreamableManager();

	TArray<FSoftObjectPath> ReferenceList;

	for (auto& Asset : Assets)
	{
		ReferenceList.Add(Asset.ToSoftObjectPath());	
	}
	AssetLoader.RequestAsyncLoad(ReferenceList, FStreamableDelegate::CreateStatic(&UCIS_UtilityFunctionLibrary::AsyncLoadClassObjectComplete, WorldContextObject, Assets, ReferenceList, OnAsyncLoadObjectCompleteDelagate));
}

void UCIS_UtilityFunctionLibrary::AsyncLoadClassObjectComplete(UObject* WorldContextObject, TArray<TSoftClassPtr<>> Assets,
	TArray<FSoftObjectPath> References, FAsyncLoadClassObjectDelegate* Callback)
{
	TArray<TSubclassOf<UObject>> AssetList;
	
	for (auto& AssetPtr : Assets)
	{
		TSubclassOf<UObject> Asset = AssetPtr.Get();
		if (!Asset) return;
		AssetList.Add(Asset);
	}
	Callback->ExecuteIfBound(AssetList);
}

bool UCIS_UtilityFunctionLibrary::ReadFromStorage(const FString& StorageName, FString& OutData)
{
	FString Path = FPaths::ProjectSavedDir()+ "UserData/" + StorageName + ".json";
	return FFileHelper::LoadFileToString(OutData, *Path);
}

void UCIS_UtilityFunctionLibrary::SaveToStorage(const FString& StorageName, const FString& Data)
{
	FString Path = FPaths::ProjectSavedDir() + "UserData/" + StorageName + ".json";
	FFileHelper::SaveStringToFile(Data, *Path);
}
