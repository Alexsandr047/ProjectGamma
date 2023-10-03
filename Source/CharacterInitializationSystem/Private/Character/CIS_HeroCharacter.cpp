// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CIS_HeroCharacter.h"

#include "AbilitySystem/CIS_AbilitySystemComponent.h"
#include "Character/CIS_CharacterMovementComponent.h"
#include "Character/CIS_PawnExtensionComponent.h"

ACIS_HeroCharacter::ACIS_HeroCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCIS_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PawnExtComponent = CreateDefaultSubobject<UCIS_PawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	CameraComponent = CreateDefaultSubobject<UCIS_CameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

UAbilitySystemComponent* ACIS_HeroCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetLiberty_AbilitySystemComponent();
}

void ACIS_HeroCharacter::SetPerspective(bool InFirstPersonPerspective)
{
	bIsFirstPersonPerspective = InFirstPersonPerspective;
}

void ACIS_HeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PawnExtComponent->HandleControllerChanged();
}

void ACIS_HeroCharacter::UnPossessed()
{
	Super::UnPossessed();
	
	PawnExtComponent->HandleControllerChanged();	
}

void ACIS_HeroCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void ACIS_HeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	PawnExtComponent->HandlePlayerStateReplicated();
}

void ACIS_HeroCharacter::UninitAndDestroy()
{
	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (UCIS_AbilitySystemComponent* CIS_ASC = GetCIS_AbilitySystemComponent())
	{
		if (CIS_ASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}
	Super::UninitAndDestroy();
}

void ACIS_HeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}
