// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Character/CIS_PawnComponent.h"
#include "Input/CIS_InputConfig.h"
#include "CIS_HeroComponent.generated.h"

class UCIS_CameraMode;
class UCIS_CameraModeConfig;

/**
 * UCIS_HeroComponent
 *
 * A component used to create a player controlled pawns (characters, vehicles, etc..).
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_HeroComponent : public UCIS_PawnComponent
{
	GENERATED_BODY()

public:
	
	UCIS_HeroComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the hero component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "CharacterInitializationSystem|Hero")
	static UCIS_HeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCIS_HeroComponent>() : nullptr); }

	void SetAbilityCameraMode(TSubclassOf<UCIS_CameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);
	void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);
	
protected:

	virtual void OnRegister() override;

	virtual bool IsPawnComponentReadyToInitialize() const override;
	void OnPawnReadyToInitialize();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//INPUT FUNCTIONS//

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);

	// Toggles between perspectives
	void TogglePerspective(const FInputActionValue& InputActionValue);
	
private:
	
	TSubclassOf<UCIS_CameraMode> DetermineCameraMode() const;
	
protected:

	/**
	 * Input Configs that should be added to this player when initalizing the input.
	 * Input Configs должны быть добавлены этому играку когда происходит инициализация ввода
	 * 
	 * NOTE: You should only add to this if you do not have a game feature plugin accessible to you.
	 * If you do, then use the GameFeatureAction_AddInputConfig instead.
	 *
	 * NOTE: Необходимо использовать если не используеться с необходимыми игровыми фунциями
	 * Если плагин есть то нужно испрользоваеть GameFeatureAction_AddInputConfig ((этого класс не реализован) этот класс используеться при взаимодейсвие с UI где происходит биндиг клавиш)
	 */

	//Default Camera Mode
	UPROPERTY(EditDefaultsOnly, Category = "CharacterInitializationSystem|Camera")
	UCIS_CameraModeConfig* CameraModes;
		
	TSubclassOf<UCIS_CameraMode> CurrentCameraMode;
	
	// Spec handle for the last ability to set a camera mode.
	FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterInitializationSystem|Input Config")
	UCIS_InputConfig* InputConfig;

	template <class UserClass, typename FuncType>
	void BindNativeAction(UEnhancedInputComponent* InputComponent, const UCIS_InputConfig* InputConfig, const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(UEnhancedInputComponent* InputComponent, const UCIS_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
	
	// True when the pawn has fully finished initialization
	bool bPawnHasInitialized;

	// True when player input bindings have been applyed, will never be true for non-playersen
	bool bReadyToBindInputs;
};
template <class UserClass, typename FuncType>
void UCIS_HeroComponent::BindNativeAction(UEnhancedInputComponent* InputComponent, const UCIS_InputConfig* InputConfig, const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		check(InputComponent);
		InputComponent->BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UCIS_HeroComponent::BindAbilityActions(UEnhancedInputComponent* InputComponent, const UCIS_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FCIS_InputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				check(InputComponent);
				BindHandles.Add(InputComponent->BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				check(InputComponent);
				BindHandles.Add(InputComponent->BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}