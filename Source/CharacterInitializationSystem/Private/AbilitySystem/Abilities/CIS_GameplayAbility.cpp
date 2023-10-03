// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CIS_GameplayAbility.h"

#include "AbilitySystemLog.h"
#include "AbilitySystem/CIS_AbilitySourceInterface.h"
#include "AbilitySystem/CIS_AbilitySystemComponent.h"
#include "AbilitySystem/CIS_GameplayEffectContext.h"
#include "Camera/CIS_CameraMode.h"
#include "Character/CIS_HeroComponent.h"
#include "Core/CIS_GameplayTags.h"

UCIS_GameplayAbility::UCIS_GameplayAbility(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	ActivationPolicy = ECIS_AbilityActivationPolicy::OnInputTriggered;
	ActivationGroup = ECIS_AbilityActivationGroup::Independent;

	//TODO NeedFinish
	//ActiveCameraMode = nullptr;
}

UCIS_AbilitySystemComponent* UCIS_GameplayAbility::GetCIS_AbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UCIS_AbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ACIS_PlayerController* UCIS_GameplayAbility::GetCIS_PlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ACIS_PlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

ACIS_Character* UCIS_GameplayAbility::GetCIS_CharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ACIS_Character>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

AController* UCIS_GameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

void UCIS_GameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec) const
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);

	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == ECIS_AbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);
			
			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UCIS_GameplayAbility::OnPawnAvatarSet()
{
	K2_OnPawnAvatarSet();
}

UCIS_HeroComponent* UCIS_GameplayAbility::GetHeroComponentFromActorInfo() const
{
	return (CurrentActorInfo ? UCIS_HeroComponent::FindHeroComponent(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

bool UCIS_GameplayAbility::CanChangeActivationGroup(ECIS_AbilityActivationGroup NewGroup) const
{
	if (!IsInstantiated() || !IsActive())
	{
		return false;
	}

	if (ActivationGroup == NewGroup)
	{
		return true;
	}

	UCIS_AbilitySystemComponent* LyraASC = GetCIS_AbilitySystemComponentFromActorInfo();
	check(LyraASC);

	if ((ActivationGroup != ECIS_AbilityActivationGroup::Exclusive_Blocking) && LyraASC->IsActivationGroupBlocked(NewGroup))
	{
		// This ability can't change groups if it's blocked (unless it is the one doing the blocking).
		return false;
	}

	if ((NewGroup == ECIS_AbilityActivationGroup::Exclusive_Replaceable) && !CanBeCanceled())
	{
		// This ability can't become replaceable if it can't be canceled.
		return false;
	}

	return true;
}

bool UCIS_GameplayAbility::ChangeActivationGroup(ECIS_AbilityActivationGroup NewGroup)
{
	ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(ChangeActivationGroup, false);

	if (!CanChangeActivationGroup(NewGroup))
	{
		return false;
	}

	if (ActivationGroup != NewGroup)
	{
		UCIS_AbilitySystemComponent* LyraASC = GetCIS_AbilitySystemComponentFromActorInfo();
		check(LyraASC);

		LyraASC->RemoveAbilityFromActivationGroup(ActivationGroup, this);
		LyraASC->AddAbilityToActivationGroup(NewGroup, this);

		ActivationGroup = NewGroup;
	}

	return true;
}

void UCIS_GameplayAbility::NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
{
	bool bSimpleFailureFound = false;
	for (FGameplayTag Reason : FailedReason)
	{
		// if (!bSimpleFailureFound)
		// {
		// 	if (const FText* pUserFacingMessage = FailureTagToUserFacingMessages.Find(Reason))
		// 	{
		// 		FLyraAbilitySimpleFailureMessage Message;
		// 		Message.PlayerController = GetActorInfo().PlayerController.Get();
		// 		Message.FailureTags = FailedReason;
		// 		Message.UserFacingReason = *pUserFacingMessage;
		//
		// 		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		// 		MessageSystem.BroadcastMessage(TAG_ABILITY_SIMPLE_FAILURE_MESSAGE, Message);
		// 		bSimpleFailureFound = true;
		// 	}
		// }
		//
		// if (UAnimMontage* pMontage = FailureTagToAnimMontage.FindRef(Reason))
		// {
		// 	FLyraAbilityMontageFailureMessage Message;
		// 	Message.PlayerController = GetActorInfo().PlayerController.Get();
		// 	Message.FailureTags = FailedReason;
		// 	Message.FailureMontage = pMontage;
		//
		// 	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		// 	MessageSystem.BroadcastMessage(TAG_ABILITY_PLAY_MONTAGE_FAILURE_MESSAGE, Message);
		// }
	}
}

void UCIS_GameplayAbility::SetCameraMode(TSubclassOf<UCIS_CameraMode> CameraMode)
{
	ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(SetCameraMode, );

	if (UCIS_HeroComponent* HeroComponent = GetHeroComponentFromActorInfo())
	{
		HeroComponent->SetAbilityCameraMode(CameraMode, CurrentSpecHandle);
		ActiveCameraMode = CameraMode;
	}
}

void UCIS_GameplayAbility::ClearCameraMode()
{
	ENSURE_ABILITY_IS_INSTANTIATED_OR_RETURN(ClearCameraMode, );

	if (ActiveCameraMode)
	{
		if (UCIS_HeroComponent* HeroComponent = GetHeroComponentFromActorInfo())
		{
			HeroComponent->ClearAbilityCameraMode(CurrentSpecHandle);
		}

		ActiveCameraMode = nullptr;
	}
}

bool UCIS_GameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                              const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}
	
	UCIS_AbilitySystemComponent* CIS_ASC = CastChecked<UCIS_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	const FCIS_GameplayTags& GameplayTags = FCIS_GameplayTags::Get();

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	//@TODO Possibly remove after setting up tag relationships
	if (CIS_ASC->IsActivationGroupBlocked(ActivationGroup))
	{
		if (OptionalRelevantTags)
		{
			OptionalRelevantTags->AddTag(GameplayTags.Ability_ActivateFail_ActivationGroup);
		}
		return false;
	}
	return true;
}

void UCIS_GameplayAbility::SetCanBeCanceled(bool bCanBeCanceled)
{
		// The ability can not block canceling if it's replaceable.
		if (!bCanBeCanceled && (ActivationGroup == ECIS_AbilityActivationGroup::Exclusive_Replaceable))
		{
			UE_LOG(LogTemp, Error, TEXT("SetCanBeCanceled: Ability [%s] can not block canceling because its activation group is replaceable."), *GetName());
			return;
		}

		Super::SetCanBeCanceled(bCanBeCanceled);
}

void UCIS_GameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	K2_OnAbilityAdded();

	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UCIS_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCIS_GameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	//ClearCameraMode();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FGameplayEffectContextHandle UCIS_GameplayAbility::MakeEffectContext(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo) const
{
	FGameplayEffectContextHandle ContextHandle = Super::MakeEffectContext(Handle, ActorInfo);

	FCIS_GameplayEffectContext* EffectContext = FCIS_GameplayEffectContext::ExtractEffectContext(ContextHandle);
	check(EffectContext);

	check(ActorInfo);

	AActor* EffectCauser = nullptr;
	const ICIS_AbilitySourceInterface* AbilitySource = nullptr;
	float SourceLevel = 0.0f;
	GetAbilitySource(Handle, ActorInfo, /*out*/ SourceLevel, /*out*/ AbilitySource, /*out*/ EffectCauser);

	UObject* SourceObject = GetSourceObject(Handle, ActorInfo);

	AActor* Instigator = ActorInfo ? ActorInfo->OwnerActor.Get() : nullptr;

	EffectContext->SetAbilitySource(AbilitySource, SourceLevel);
	EffectContext->AddInstigator(Instigator, EffectCauser);
	EffectContext->AddSourceObject(SourceObject);

	return ContextHandle;
}

void UCIS_GameplayAbility::GetAbilitySource(FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel,
	const ICIS_AbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser) const
{
	OutSourceLevel = 0.0f;
	OutAbilitySource = nullptr;
	OutEffectCauser = nullptr;

	OutEffectCauser = ActorInfo->AvatarActor.Get();

	// If we were added by something that's an ability info source, use it
	UObject* SourceObject = GetSourceObject(Handle, ActorInfo);

	OutAbilitySource = Cast<ICIS_AbilitySourceInterface>(SourceObject);
}
	
