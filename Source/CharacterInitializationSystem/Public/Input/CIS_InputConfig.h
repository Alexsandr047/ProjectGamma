// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Engine/DataAsset.h"
#include "CIS_InputConfig.generated.h"

/*
CHARACTERINITIALIZATIONSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Look_Mouse);
CHARACTERINITIALIZATIONSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Move);
CHARACTERINITIALIZATIONSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Crouch);
*/

class UInputAction;
class UInputMappingContext;
class UCIS_LocalPlayer;

/**
 * FCIS_InputAction
 *
 *	Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FCIS_InputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	UCIS_InputConfig(const FObjectInitializer& ObjectInitializer);

	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:

	//INPUT MAPPING//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Meta = (TitleProperty = "InputMapping"))
	UInputMappingContext* MappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Meta = (TitleProperty = "InputMapping"))
	int32 BaseMappingPriority;
	
	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FCIS_InputAction> NativeInputActions;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FCIS_InputAction> AbilityInputActions;
};
