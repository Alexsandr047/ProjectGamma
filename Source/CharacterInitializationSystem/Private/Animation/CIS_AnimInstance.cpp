// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CIS_AnimInstance.h"

#include "AbilitySystemGlobals.h"
#include "Character/CIS_Character.h"
#include "Character/CIS_CharacterMovementComponent.h"

UCIS_AnimInstance::UCIS_AnimInstance(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

void UCIS_AnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}
#if WITH_EDITOR
EDataValidationResult UCIS_AnimInstance::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	GameplayTagPropertyMap.IsDataValid(this, ValidationErrors);

	return ((ValidationErrors.Num() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void UCIS_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UCIS_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const ACIS_Character* Character = Cast<ACIS_Character>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	UCIS_CharacterMovementComponent* CharMoveComp = CastChecked<UCIS_CharacterMovementComponent>(Character->GetCharacterMovement());
	const FCIS_CharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}
