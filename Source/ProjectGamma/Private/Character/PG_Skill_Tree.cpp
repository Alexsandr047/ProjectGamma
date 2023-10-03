// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PG_Skill_Tree.h"

UPG_Skill::UPG_Skill(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

UPG_Skill_Tree::UPG_Skill_Tree(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

UPG_Skill* UPG_Skill_Tree::MakeSkill(const FString& SkillID, TArray<UPG_Skill*> ParentSkill)
{
	if(UPG_Skill* NewSkill = NewObject<UPG_Skill>(this))
	{
		NewSkill->SkillData.SkillId = SkillID;
		NewSkill->Parents = ParentSkill;
		Skills.Add(SkillID, NewSkill);
		return NewSkill;
	}
	return nullptr;
}

ESkillStatus UPG_Skill_Tree::CheckStatusSkill(const FString& Skill)
{
	UPG_Skill* CS = *Skills.Find(Skill);
	if(CS)
	{
		if(CS->SkillData.Unlock)
			return ESkillStatus::Unlock;
	
		if(CS->Parents.Num()>0)
		{
			for(auto& ParentSkill : CS->Parents)
			{
				if(ParentSkill->SkillData.Unlock)
				{
					return ESkillStatus::CanUnlock;
				}
			}					
		}
		else
			return ESkillStatus::CanUnlock;
	}
	return ESkillStatus::Lock;
}

void UPG_Skill_Tree::ParseData(TArray<FSkillParseData> _Skills)
{
	SetSkillDependencies();
	if(Skills.Num() > 0)
	{
		for(auto& Skill : _Skills)
		{
			if(UPG_Skill* SkillClass = *Skills.Find(Skill.SkillId))
				SkillClass->SkillData.Unlock = Skill.Unlock;
		}
	}
}

TArray<FSkillParseData> UPG_Skill_Tree::GetSkillsData()
{
	TArray<FSkillParseData> SkillParseData;
	for(auto& Skill :Skills)
	{
		SkillParseData.Add(FSkillParseData(Skill.Value->SkillData.SkillId, Skill.Value->SkillData.Unlock));
	}
	return SkillParseData;
}
