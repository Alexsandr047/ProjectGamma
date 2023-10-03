// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "CIS_ItemDefinition.generated.h"

class UCIS_ItemInstance;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_ItemFragment : public UObject
{
	GENERATED_BODY()

public:

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;

	
	virtual void OnInstanceCreated(UCIS_ItemInstance* Instance) const {}
};



UCLASS(Blueprintable)
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_ItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;
	
	UCIS_ItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditAnywhere,Category=Equipment)
	FGameplayTagContainer ItemTags;

	UFUNCTION(BlueprintPure, Category=Equipment)
	FGameplayTagContainer GetItemTags() const { return ItemTags; }
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category=Display, Instanced)
	TArray<UCIS_ItemFragment*> Fragments;

public:
	UCIS_ItemFragment* FindFragmentByClass(TSubclassOf<UCIS_ItemFragment> FragmentClass) const;
};


UCLASS()
class UCISInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UCIS_ItemFragment* FindItemDefinitionFragment(TSubclassOf<UCIS_ItemDefinition> ItemDef, TSubclassOf<UCIS_ItemFragment> FragmentClass);
};
