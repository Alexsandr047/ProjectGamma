// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CIS_Character.h"
#include "CIS_HeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API ACIS_HeroCharacter : public ACIS_Character
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ACIS_HeroCharacter(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
#pragma region PawnExtension
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterInitializationSystem|Character", Meta = (AllowPrivateAccess = "true"))
	UCIS_PawnExtensionComponent* PawnExtComponent;

#pragma endregion
#pragma region Camera
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterInitializationSystem|Character", Meta = (AllowPrivateAccess = "true"))
	UCIS_CameraComponent* CameraComponent;

	UFUNCTION(BlueprintCallable, Category = "CharacterInitializationSystem|Character")
	UCIS_CameraComponent* GetCameraComponent() const { return CameraComponent; }
	
	UPROPERTY(BlueprintReadOnly, Category = "CharacterInitializationSystem|Camera")
	bool bIsFirstPersonPerspective = false;

	UFUNCTION(BlueprintCallable, Category = "CharacterInitializationSystem|Camera")
	bool GetIsFirstPersonPerspective() const { return bIsFirstPersonPerspective; }
	
	void SetPerspective(bool InFirstPersonPerspective);

	UPROPERTY(BlueprintAssignable, Category="Impact")
	FOnSwapCameraDelegate OnSwapCameraDelegate;
	

#pragma endregion

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	
	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual void UninitAndDestroy() override;
	
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
