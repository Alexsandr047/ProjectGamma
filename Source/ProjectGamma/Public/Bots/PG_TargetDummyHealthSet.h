// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSet/CIS_HealthSet.h"
#include "PG_TargetDummyHealthSet.generated.h"

/**
 * PG Target Dummy Health Set
 *
 * This attribute set will not allow the Target Dummy to go below 1 Health.
 */
UCLASS()
class PROJECTGAMMA_API UPG_TargetDummyHealthSet : public UCIS_HealthSet
{
	GENERATED_BODY()
	
public:
	// Set Class Defaults
	UPG_TargetDummyHealthSet();

	//~ULyraHealthSet implementation
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	//~End of ULyraHealthSet implementation
};
