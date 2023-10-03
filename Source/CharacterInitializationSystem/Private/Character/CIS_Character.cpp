// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CIS_Character.h"

#include "AbilitySystem/CIS_AbilitySet.h"
#include "AbilitySystem/CIS_AbilitySystemComponent.h"
#include "Character/CIS_CharacterMovementComponent.h"
#include "Character/CIS_HealthComponent.h"
#include "Character/CIS_PawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/CIS_GameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "Player/CIS_PlayerState.h"

// Sets default values
ACIS_Character::ACIS_Character(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCIS_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 900000000.0f;
	


	HealthComponent = CreateDefaultSubobject<UCIS_HealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);
	
	UCIS_CharacterMovementComponent* CIS_MoveComp = CastChecked<UCIS_CharacterMovementComponent>(GetCharacterMovement());
	CIS_MoveComp->GravityScale = 1.0f;
	CIS_MoveComp->MaxAcceleration = 2400.0f;
	CIS_MoveComp->BrakingFrictionFactor = 1.0f;
	CIS_MoveComp->BrakingFriction = 6.0f;
	CIS_MoveComp->GroundFriction = 8.0f;
	CIS_MoveComp->BrakingDecelerationWalking = 1400.0f;
	CIS_MoveComp->bUseControllerDesiredRotation = false;
	CIS_MoveComp->bOrientRotationToMovement = false;
	CIS_MoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	CIS_MoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	CIS_MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	CIS_MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	CIS_MoveComp->SetCrouchedHalfHeight(65.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
}

void ACIS_Character::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, ReplicatedAcceleration, COND_SimulatedOnly);
}


void ACIS_Character::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

// Called when the game starts or when spawned
void ACIS_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACIS_Character::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACIS_Character::Reset()
{
	DisableMovementAndCollision();

	K2_OnReset();

	UninitAndDestroy();
}

void ACIS_Character::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
		const double MaxAccel = MovementComponent->MaxAcceleration;
		const FVector CurrentAccel = MovementComponent->GetCurrentAcceleration();
		double AccelXYRadians, AccelXYMagnitude;
		FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);

		ReplicatedAcceleration.AccelXYRadians   = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);     // [0, 2PI] -> [0, 255]
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
		ReplicatedAcceleration.AccelZ           = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);   // [-MaxAccel, MaxAccel] -> [-127, 127]
	}
}

ACIS_PlayerState* ACIS_Character::GetCIS_PlayerState() const
{
	return CastChecked<ACIS_PlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

void ACIS_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ACIS_Character::UnPossessed()
{
	Super::UnPossessed();
}

void ACIS_Character::OnRep_Controller()
{
	Super::OnRep_Controller();
}

void ACIS_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ACIS_Character::InitializeGameplayTags()
{
	// Clear tags that may be lingering on the ability system from the previous pawn.
	if (UCIS_AbilitySystemComponent* CIS_ASC = GetCIS_AbilitySystemComponent())
	{
		const FCIS_GameplayTags& GameplayTags = FCIS_GameplayTags::Get();

		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags.MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				CIS_ASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags.CustomMovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				CIS_ASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		UCIS_CharacterMovementComponent* CISMoveComp = CastChecked<UCIS_CharacterMovementComponent>(GetCharacterMovement());
		SetMovementModeTag(CISMoveComp->MovementMode, CISMoveComp->CustomMovementMode, true);
	}
}

void ACIS_Character::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void ACIS_Character::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ACIS_Character::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	UCIS_CharacterMovementComponent* CIS_MoveComp = CastChecked<UCIS_CharacterMovementComponent>(GetCharacterMovement());

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(CIS_MoveComp->MovementMode, CIS_MoveComp->CustomMovementMode, true);
}

void ACIS_Character::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UCIS_AbilitySystemComponent* CIS_ASC = GetCIS_AbilitySystemComponent())
	{
		const FCIS_GameplayTags& GameplayTags = FCIS_GameplayTags::Get();
		const FGameplayTag* MovementModeTag = nullptr;

		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = GameplayTags.CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = GameplayTags.MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			CIS_ASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

void ACIS_Character::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UCIS_CharacterMovementComponent* CISMoveComp = CastChecked<UCIS_CharacterMovementComponent>(GetCharacterMovement());
	CISMoveComp->StopMovementImmediately();
	CISMoveComp->DisableMovement();
}

void ACIS_Character::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void ACIS_Character::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (UCIS_AbilitySystemComponent* CIS_ASC = GetCIS_AbilitySystemComponent())
		GrantedHandlesOnSpawn.TakeFromAbilitySystem(CIS_ASC);

	SetActorHiddenInGame(true);
}

void ACIS_Character::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UCIS_AbilitySystemComponent* CIS_ASC = GetCIS_AbilitySystemComponent())
	{
		CIS_ASC->SetLooseGameplayTagCount(FCIS_GameplayTags::Get().Status_Crouching, 1);
	}

	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ACIS_Character::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UCIS_AbilitySystemComponent* CIS_ASC = GetCIS_AbilitySystemComponent())
	{
		CIS_ASC->SetLooseGameplayTagCount(FCIS_GameplayTags::Get().Status_Crouching, 0);
	}

	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

bool ACIS_Character::CanJumpInternal_Implementation() const
{
	// same as ACharacter's implementation but without the crouch check
	return Super::CanJumpInternal_Implementation();
}

void ACIS_Character::OnRep_ReplicatedAcceleration()
{
	if (UCIS_CharacterMovementComponent* CISMovementComponent = Cast<UCIS_CharacterMovementComponent>(GetCharacterMovement()))
	{
		// Decompress Acceleration
		const double MaxAccel         = CISMovementComponent->MaxAcceleration;
		const double AccelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0; // [0, 255] -> [0, MaxAccel]
		const double AccelXYRadians   = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;     // [0, 255] -> [0, 2PI]

		FVector UnpackedAcceleration(FVector::ZeroVector);
		FMath::PolarToCartesian(AccelXYMagnitude, AccelXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0; // [-127, 127] -> [-MaxAccel, MaxAccel]

		CISMovementComponent->SetReplicatedAcceleration(UnpackedAcceleration);
	}
}

UCIS_AbilitySystemComponent* ACIS_Character::GetCIS_AbilitySystemComponent() const
{
	return Cast<UCIS_AbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ACIS_Character::GetAbilitySystemComponent() const
{
	//AbilitySystemComponent must be initiated in the child classes
	return nullptr;
}

void ACIS_Character::OnAbilitySystemInitialized()
{
	UCIS_AbilitySystemComponent* CIS_ASC = GetCIS_AbilitySystemComponent();
	check(CIS_ASC);

	if (HasAuthority() && AbilitySet)
		AbilitySet->GiveToAbilitySystem(GetCIS_AbilitySystemComponent(), &GrantedHandlesOnSpawn);
	
	HealthComponent->InitializeWithAbilitySystem(CIS_ASC);

	InitializeGameplayTags();
}

void ACIS_Character::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeFromAbilitySystem();
}

// Called every frame
void ACIS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACIS_Character::ToggleCrouch()
{
	
	const UCIS_CharacterMovementComponent* CISMoveComp = CastChecked<UCIS_CharacterMovementComponent>(GetCharacterMovement());
	
	if (bIsCrouched || CISMoveComp->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (CISMoveComp->IsMovingOnGround())
	{
		Crouch();
	}
}

