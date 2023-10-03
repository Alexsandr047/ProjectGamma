// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/PG_CoreTypes.h"
#include "UObject/NoExportTypes.h"
#include "PG_Skill_Tree.generated.h"

USTRUCT(BlueprintType)
struct FSkill 
{
	
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillParseData SkillData;
	
	UPROPERTY()
	TArray<FSkillParseData> Parents;

	FSkill()
	{
		SkillData = FSkillParseData();
		Parents.Empty();
	}
	FSkill(FSkillParseData ParseData)
	{
		SkillData = ParseData;
	}
	FSkill(FSkillParseData ParseData, TArray<FSkillParseData> ParentsData)
	{
		SkillData = ParseData;
		Parents = ParentsData;
	}

	bool IsValid() const
	{
		if(SkillData.SkillId != "")
			return true;
		return false;
	}
};

UENUM(BlueprintType)
enum class ESkillStatus : uint8
{
	Lock,
	CanUnlock,
	Unlock
};

UCLASS(BlueprintType, Blueprintable)
class PROJECTGAMMA_API UPG_Skill : public UObject
{
	GENERATED_BODY()
	
public:

	UPG_Skill(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillParseData SkillData;

	UPROPERTY()
	TArray<UPG_Skill*> Parents;
	
};
UCLASS(BlueprintType, Blueprintable)
class PROJECTGAMMA_API UPG_Skill_Tree : public UObject
{
	GENERATED_BODY()
	
	UPG_Skill_Tree(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
public:
	UPROPERTY()
	TMap<FString, UPG_Skill*> Skills;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetSkillDependencies();
	
	UFUNCTION(BlueprintCallable)
	UPG_Skill* MakeSkill(const FString& SkillID, TArray<UPG_Skill*> ParentSkill);

	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "ReturnValue"))
	ESkillStatus CheckStatusSkill(const FString& Skill);

	void ParseData(TArray<FSkillParseData> Skills);

	TArray<FSkillParseData> GetSkillsData();
};
