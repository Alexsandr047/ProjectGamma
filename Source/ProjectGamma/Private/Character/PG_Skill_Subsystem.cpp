// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PG_Skill_Subsystem.h"

void UPG_Skill_Subsystem::InitSkillTree(const FString& SkillTreeID, TArray<FSkillParseData> Skills,
	TSubclassOf<UPG_Skill_Tree> SkillTree)
{
	if(!SkillTree)
		return;

	if(UPG_Skill_Tree* Tree = NewObject<UPG_Skill_Tree>(this, SkillTree))
	{		
		Tree->ParseData(Skills);
		SkillTrees.Add(SkillTreeID, Tree);
	}
}

UPG_Skill_Tree* UPG_Skill_Subsystem::GetSkillTreeById(const FString& SkillTreeID) const
{
	return *SkillTrees.Find(SkillTreeID);
}
