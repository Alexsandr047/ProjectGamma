// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/PG_CharacterPresets.h"
#include "PG_CharacterPresets_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTGAMMA_API UPG_CharacterPresets_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPG_CharacterPresets_Component();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void InitCharacterPreset();

	UPROPERTY(ReplicatedUsing=OnRep_UpdateCharacterPreset)
	FCharacterPresets UpdateCharacterPreset;
	
	UFUNCTION()
	void OnRep_UpdateCharacterPreset();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
