// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CIS_CoreTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CIS_UtilityFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_UtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	static void AsyncLoadObject(UObject* WorldContextObject, const TArray<TSoftObjectPtr<UObject>>& Assets, FAsyncLoadObjectDelegate* OnAsyncLoadObjectCompleteDelagate);
	static void AsyncLoadObjectComplete(UObject* WorldContextObject, TArray<TSoftObjectPtr<UObject>> Assets, TArray<FSoftObjectPath> References, FAsyncLoadObjectDelegate* Callback);

	static void AsyncLoadClassObject(UObject* WorldContextObject, const TArray<TSoftClassPtr<UObject>>& Assets, FAsyncLoadClassObjectDelegate* OnAsyncLoadObjectCompleteDelagate);
	static void AsyncLoadClassObjectComplete(UObject* WorldContextObject, TArray<TSoftClassPtr<UObject>> Assets, TArray<FSoftObjectPath> References, FAsyncLoadClassObjectDelegate* Callback);


	UFUNCTION(BlueprintCallable)
	static bool ReadFromStorage(const FString& StorageName, FString& OutData);
	UFUNCTION(BlueprintCallable)
	static void SaveToStorage(const FString& StorageName, const FString& Data);
};
