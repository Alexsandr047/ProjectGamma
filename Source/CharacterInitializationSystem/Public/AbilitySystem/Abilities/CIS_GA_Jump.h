// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CIS_GameplayAbility.h"
#include "CIS_GA_Jump.generated.h"

/**
 * UCIS_GameplayAbility_Jump
 *
 *	Gameplay ability used for character jumping.
 */
UCLASS(Abstract)
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_GA_Jump : public UCIS_GameplayAbility
{
	GENERATED_BODY()
	
	public:

		UCIS_GA_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	protected:

		virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
		virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

		UFUNCTION(BlueprintCallable, Category = "Liberty|Ability")
		void CharacterJumpStart();

		UFUNCTION(BlueprintCallable, Category = "Liberty|Ability")
		void CharacterJumpStop();
};
