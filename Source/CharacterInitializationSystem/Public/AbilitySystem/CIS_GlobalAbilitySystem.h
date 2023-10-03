
#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayAbilitySpec.h"
#include "Subsystems/WorldSubsystem.h"
#include "CIS_GlobalAbilitySystem.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UCIS_AbilitySystemComponent;

USTRUCT()
struct FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<UCIS_AbilitySystemComponent*, FGameplayAbilitySpecHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UCIS_AbilitySystemComponent* ASC);
	void RemoveFromASC(UCIS_AbilitySystemComponent* ASC);
	void RemoveFromAll();
};

USTRUCT()
struct FGlobalAppliedEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<UCIS_AbilitySystemComponent*, FActiveGameplayEffectHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UCIS_AbilitySystemComponent* ASC);
	void RemoveFromASC(UCIS_AbilitySystemComponent* ASC);
	void RemoveFromAll();
};


UCLASS()
class CHARACTERINITIALIZATIONSYSTEM_API UCIS_GlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UCIS_GlobalAbilitySystem();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="CharacterInitializationSystem")
	void ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="CharacterInitializationSystem")
	void ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "CharacterInitializationSystem")
	void RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "CharacterInitializationSystem")
	void RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect);
	

	/** Register an ASC with global system and apply any active global effects/abilities. */
	void RegisterASC(UCIS_AbilitySystemComponent* ASC);
	
	/** Removes an ASC from the global system, along with any active global effects/abilities. */
	void UnregisterASC(UCIS_AbilitySystemComponent* ASC);

private:
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	UPROPERTY()
	TArray<UCIS_AbilitySystemComponent*> RegisteredASCs;
};
