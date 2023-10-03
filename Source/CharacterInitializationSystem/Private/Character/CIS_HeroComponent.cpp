// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CIS_HeroComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/CIS_AbilitySystemComponent.h"
#include "Camera/CIS_CameraComponent.h"
#include "Camera/CIS_CameraMode.h"
#include "Camera/CIS_CameraModeConfig.h"
#include "Character/CIS_Character.h"
#include "Character/CIS_PawnExtensionComponent.h"
#include "Core/CIS_GameplayTags.h"
#include "Player/CIS_PlayerController.h"
#include "Player/CIS_PlayerState.h"


UCIS_HeroComponent::UCIS_HeroComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CurrentCameraMode = nullptr;
	bPawnHasInitialized = false;
	bReadyToBindInputs = false;
}

void UCIS_HeroComponent::SetAbilityCameraMode(TSubclassOf<UCIS_CameraMode> CameraMode,
	const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		CurrentCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UCIS_HeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		CurrentCameraMode =  nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

void UCIS_HeroComponent::OnRegister()
{
	Super::OnRegister();

	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (UCIS_PawnExtensionComponent* PawnExtComp = UCIS_PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnExtComp->OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnPawnReadyToInitialize));
		}
	}
}

bool UCIS_HeroComponent::IsPawnComponentReadyToInitialize() const
{
	// The player state is required.
	if (!GetPlayerState<ACIS_PlayerState>())
	{
		return false;
	}

	const APawn* Pawn = GetPawn<APawn>();
	
	// A pawn is required.
	if (!Pawn)
	{
		return false;
	}

	// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
	if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
	{
		AController* Controller = GetController<AController>();

		const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
												(Controller->PlayerState != nullptr) && \
												(Controller->PlayerState->GetOwner() == Controller);

		if (!bHasControllerPairedWithPS)
		{
			return false;
		}
	}

	const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

	//const bool bIsBot = Pawn->IsBotControlled();

	if (bIsLocallyControlled /*&& !bIsBot*/)
	{
		// The input component is required when locally controlled.
		if (!Pawn->InputComponent)
		{
			return false;
		}
	}
	
	return true;
}

void UCIS_HeroComponent::OnPawnReadyToInitialize()
{
	if (!ensure(!bPawnHasInitialized))
	{
		// Don't initialize twice
		return;
	}

	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

	ACIS_PlayerState* Liberty_PS = GetPlayerState<ACIS_PlayerState>();
	check(Liberty_PS);


	if (UCIS_PawnExtensionComponent* PawnExtComp = UCIS_PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
		// The ability system component and attribute sets live on the player state.
		PawnExtComp->InitializeAbilitySystem(Liberty_PS->GetCIS_AbilitySystemComponent(), Liberty_PS);
	}
	
	if (ACIS_PlayerController* PC = GetController<ACIS_PlayerController>())
	{
		if (Pawn->InputComponent != nullptr)
		{
			InitializePlayerInput(Pawn->InputComponent);
		}
	}
	
	ACIS_Character* Character = Cast<ACIS_Character>(Pawn);
	if (Character)
	{
		ACIS_PlayerController* PC = GetController<ACIS_PlayerController>();
		if(PC && PC->IsLocalController())
		{
			/*if(Character->GetIsFirstPersonPerspective())
				CurrentCameraMode = CameraModes->GetFirstAndThirdMode().FirstCameraMode;
			else*/
				CurrentCameraMode = CameraModes->GetFirstAndThirdMode().ThirdCameraMode;
		}
	}

	if (bIsLocallyControlled && CurrentCameraMode)
	{
		if (UCIS_CameraComponent* CameraComponent = UCIS_CameraComponent::FindCameraComponent(Pawn))
		{			
			CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
		}
	}
	
	bPawnHasInitialized = true;
}

void UCIS_HeroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCIS_HeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (UCIS_PawnExtensionComponent* PawnExtComp = UCIS_PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnExtComp->UninitializeAbilitySystem();
		}	
	}
	
	Super::EndPlay(EndPlayReason);
}

void UCIS_HeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
	Subsystem->ClearAllMappings();

	if (const UCIS_PawnExtensionComponent* PawnExtComp = UCIS_PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if(UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			if(InputConfig)
			{
				const FCIS_GameplayTags& GameplayTags = FCIS_GameplayTags::Get();
				
				// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
				Subsystem->AddMappingContext(InputConfig->MappingContext, InputConfig->BaseMappingPriority);

				TArray<uint32> BindHandles;
				BindAbilityActions(PlayerEnhancedInputComponent, InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

				
				BindNativeAction(PlayerEnhancedInputComponent, InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
				BindNativeAction(PlayerEnhancedInputComponent, InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
				BindNativeAction(PlayerEnhancedInputComponent, InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
				BindNativeAction(PlayerEnhancedInputComponent, InputConfig, GameplayTags.InputTag_TogglePerspective, ETriggerEvent::Triggered, this, &ThisClass::TogglePerspective, /*bLogIfNotFound=*/ false);
			}
		}
	}
}

void UCIS_HeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UCIS_PawnExtensionComponent* PawnExtComp = UCIS_PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UCIS_AbilitySystemComponent* CIS_ASC = PawnExtComp->GetLiberty_AbilitySystemComponent())
			{
				CIS_ASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void UCIS_HeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UCIS_PawnExtensionComponent* PawnExtComp = UCIS_PawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UCIS_AbilitySystemComponent* CIS_ASC = PawnExtComp->GetLiberty_AbilitySystemComponent())
		{
			CIS_ASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UCIS_HeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UCIS_HeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UCIS_HeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (ACIS_Character* Character = GetPawn<ACIS_Character>())
	{
		Character->ToggleCrouch();
	}
}

void UCIS_HeroComponent::TogglePerspective(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	ACIS_Character* Character = Cast<ACIS_Character>(Pawn);
	if (Character)
	{
		//TODO Camera
		//const bool bIsFirstPersonPerspective = Character->GetIsFirstPersonPerspective();
		
		//Character->SetPerspective(!bIsFirstPersonPerspective);
		
		ACIS_PlayerController* PC = GetController<ACIS_PlayerController>();
		if(PC && PC->IsLocalController())
		{
			
			/*if(Character->GetIsFirstPersonPerspective())
			{
				CurrentCameraMode = CameraModes->GetFirstAndThirdMode().FirstCameraMode;
				Character->GetMesh()->SetVisibility(false, true);
				Character->GetClientMesh()->SetVisibility(true, true);
			}
			else
			{*/
				CurrentCameraMode = CameraModes->GetFirstAndThirdMode().ThirdCameraMode;
				Character->GetMesh()->SetVisibility(true, true);
				//Character->GetClientMesh()->SetVisibility(false, true);
			//}	
		}
	}
}

TSubclassOf<UCIS_CameraMode> UCIS_HeroComponent::DetermineCameraMode() const
{
	if (CurrentCameraMode)
	{
		return CurrentCameraMode;
	}
	
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}
	
	if(CameraModes)
		return CameraModes->CameraModes.ThirdCameraMode;


	return nullptr;
}
