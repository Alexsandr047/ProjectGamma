// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "CIS_AnimInstance.generated.h"

/*
/Animation Locomotion Library
/Animation Wraping
*/
#pragma region Animation
UENUM(BlueprintType)
enum class ECIS_AnimEnum_RootYawOffsetMode : uint8
{
	BlendOut,
	Hold,
	Accumulate
};

UENUM(BlueprintType)
enum class ECIS_AnimEnum_CardinalDirection : uint8
{
	Forward,
	Forward_Left,
	Forward_Right,
	Backward,
	Backward_Left,
	Backward_Right,
	Left,
	Right
};

USTRUCT(BlueprintType)
struct FCardinalDirections
{
public:
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Forward_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Forward_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Backward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Backward_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Backward_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Right;
};
#pragma endregion


UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UCIS_AnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

protected:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif // WITH_EDITOR

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	// Gameplay tags that can be mapped to blueprint variables. The variables will automatically update as the tags are added or removed.
	// These should be used instead of manually querying for the gameplay tags.
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
