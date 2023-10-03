// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PG_Skill_Tree.h"
#include "Core/PG_CoreTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PG_Skill_Subsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAMMA_API UPG_Skill_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FString, UPG_Skill_Tree*> SkillTrees;
	
	void InitSkillTree(const FString& SkillTreeID, TArray<FSkillParseData> Skills, TSubclassOf<UPG_Skill_Tree> SkillTree);

	UFUNCTION(BlueprintCallable)
	UPG_Skill_Tree* GetSkillTreeById(const FString& SkillTreeID) const;
};
