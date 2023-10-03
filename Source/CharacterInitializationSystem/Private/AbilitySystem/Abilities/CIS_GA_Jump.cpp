// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CIS_GA_Jump.h"
#include "Character/CIS_Character.h"

UCIS_GA_Jump::UCIS_GA_Jump(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UCIS_GA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ACIS_Character* CISCharacter = Cast<ACIS_Character>(ActorInfo->AvatarActor.Get());
	if (!CISCharacter || !CISCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UCIS_GA_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCIS_GA_Jump::CharacterJumpStart()
{
	if (ACIS_Character* CISCharacter = GetCIS_CharacterFromActorInfo())
	{
		if (CISCharacter->IsLocallyControlled() && !CISCharacter->bPressedJump)
		{
			CISCharacter->UnCrouch();
			CISCharacter->Jump();
		}
	}
}

void UCIS_GA_Jump::CharacterJumpStop()
{
	if (ACIS_Character* CISCharacter = GetCIS_CharacterFromActorInfo())
	{
		if (CISCharacter->IsLocallyControlled() && CISCharacter->bPressedJump)
		{
			CISCharacter->StopJumping();
		}
	}
}
