#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/**
 * FCIS_GameplayTags
 *
 *	Singleton containing native gameplay tags.
 */

struct FCIS_GameplayTags
{
public:

	static const FCIS_GameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);

public:

	FGameplayTag Ability_ActivateFail_ActivationGroup;

	FGameplayTag Ability_Behavior_SurvivesDeath;
	
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Crouch;
	FGameplayTag InputTag_TogglePerspective;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Interact;
	
	FGameplayTag Status_Crouching;
	FGameplayTag Status_Death;
	FGameplayTag Status_Death_Dying;
	FGameplayTag Status_Death_Dead;

	FGameplayTag GameplayEvent_Death;
	FGameplayTag GameplayEvent_Reset;
	FGameplayTag GameplayEvent_RequestReset;
	
	FGameplayTag SetByCaller_Damage;
	FGameplayTag SetByCaller_Heal;

	FGameplayTag Cheat_GodMode;
	FGameplayTag Cheat_UnlimitedHealth;
	
	TMap<uint8, FGameplayTag> MovementModeTagMap;
	TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	//AbilityTags
	FGameplayTag GAT_Jump;
	FGameplayTag GAT_Interact;
	FGameplayTag GAT_InteractEnd;
	FGameplayTag GAT_InteractBlock;
	FGameplayTag GAT_StateInteractRemoval;
	FGameplayTag GAT_StateInteractState;

protected:

	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 MovementMode);
	void AddCustomMovementModeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, uint8 CustomMovementMode);

 
private:

	static FCIS_GameplayTags GameplayTags;
	
};
