// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "PG_ShipBase.generated.h"

class UInputAction;

UCLASS()
class PROJECTGAMMA_API APG_ShipBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APG_ShipBase();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ProjectGamma|Camera")
	float CameraBoomArmLength = 100.f;

	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectGamma|Camera")
	float MouseMoveSensitivity = 1.f;

	UPROPERTY()
	APlayerController * PlayerController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	/** Called for movement input */
	void MoveButtons(const FInputActionValue& Value);

	

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
